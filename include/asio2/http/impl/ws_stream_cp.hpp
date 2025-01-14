/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 */

#ifndef __ASIO2_WS_STREAM_COMPONENT_HPP__
#define __ASIO2_WS_STREAM_COMPONENT_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <memory>
#include <future>
#include <utility>
#include <string_view>

#include <asio2/external/asio.hpp>
#include <asio2/external/beast.hpp>

#include <asio2/base/error.hpp>

namespace asio2::detail
{
	template<class, class = std::void_t<>>
	struct is_websocket_client : std::false_type {};

	template<class T>
	struct is_websocket_client<T, std::void_t<decltype(std::declval<T&>().ws_stream())>> : std::true_type {};

	template<class, class = std::void_t<>>
	struct is_websocket_server : std::false_type {};

	template<class T>
	struct is_websocket_server<T, std::void_t<decltype(std::declval<typename T::session_type&>().
		ws_stream())>> : std::true_type {};

	template<class derived_t, class args_t>
	class ws_stream_cp
	{
	public:
		using stream_type = typename args_t::stream_t;

		/**
		 * @brief constructor
		 */
		ws_stream_cp() {}

		/**
		 * @brief destructor
		 */
		~ws_stream_cp() noexcept {}

		/**
		 * @brief get the websocket stream object refrence
		 */
		inline stream_type & ws_stream() noexcept
		{
			ASIO2_ASSERT(bool(this->ws_stream_));
			return (*(this->ws_stream_));
		}

	protected:
		template<typename MatchCondition, typename Socket>
		inline void _ws_init(condition_wrap<MatchCondition>& condition, Socket& socket)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			detail::ignore_unused(derive, condition, socket);

			// In previous versions, "_ws_init" maybe called multi times, when "_ws_init" was called,
			// the "_ws_stop" maybe called at the same time, but "_ws_init" and "_ws_stop" was called
			// at different threads, this will cause the diffrent threads "read, write" the "ws_stream_"
			// variable, and then cause crash.

			if constexpr (args_t::is_client)
			{
				ASIO2_ASSERT(derive.io().running_in_this_thread());
			}
			else
			{
				ASIO2_ASSERT(derive.sessions().io().running_in_this_thread());
			}

			this->ws_stream_ = std::make_unique<stream_type>(socket);

			// Set suggested timeout settings for the websocket
			if constexpr (args_t::is_session)
			{
				this->ws_stream_->set_option(websocket::stream_base::timeout::suggested(
					beast::role_type::server));
			}
			else
			{
				this->ws_stream_->set_option(websocket::stream_base::timeout::suggested(
					beast::role_type::client));
			}
		}

		template<typename MatchCondition, typename Socket>
		inline void _ws_start(
			const std::shared_ptr<derived_t>& this_ptr,
			const condition_wrap<MatchCondition>& condition, Socket& socket)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			detail::ignore_unused(derive, this_ptr, condition, socket);

			ASIO2_ASSERT(derive.io().running_in_this_thread());
		}

		template<typename DeferEvent>
		inline void _ws_stop(std::shared_ptr<derived_t> this_ptr, DeferEvent chain)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(derive.io().running_in_this_thread());

			if (!this->ws_stream_)
				return;

			// bug fixed : resolve beast::websocket::detail::soft_mutex 
			// BHO_ASSERT(id_ != T::id); failed (line 89).
			// 
			// If this assert goes off it means you are attempting to
			// simultaneously initiate more than one of same asynchronous
			// operation, which is not allowed. For example, you must wait
			// for an async_read to complete before performing another
			// async_read.
			derive.disp_event([this, &derive, this_ptr = std::move(this_ptr), e = chain.move_event()]
			(event_queue_guard<derived_t> g) mutable
			{
				// must construct a new chain
				defer_event chain(std::move(e), std::move(g));

				try
				{
					// Set the handshake timeout to a small value, otherwise if the remote don't
					// send a websocket close frame, the async_close's callback will never be
					// called.
					websocket::stream_base::timeout opt{};
					opt.handshake_timeout = std::chrono::milliseconds(ws_shutdown_timeout);
					opt.idle_timeout      = websocket::stream_base::none();
					derive.ws_stream_->set_option(opt);
				}
				catch (system_error const& e)
				{
					set_last_error(e);
				}

				// Can't call close twice
				// TODO return a custom error code
				// BHO_ASSERT(! impl.wr_close);
				if (!this->ws_stream_->is_open())
				{
					// must Reset the control frame callback. the control frame callback hold the 
					// self shared_ptr, if don't reset it, will cause memory leaks.
					this->ws_stream_->control_callback();

					return;
				}

				// Close the WebSocket connection
				// async_close behavior : 
				// send a websocket close frame to the remote, and wait for recv a websocket close
				// frame from the remote.
				this->ws_stream_->async_close(websocket::close_code::normal,
				[this, this_ptr = std::move(this_ptr), chain = std::move(chain)]
				(error_code ec) mutable
				{
					detail::ignore_unused(ec);

					//if (ec)
					//	return;

					// If we get here then the connection is closed gracefully

					// must Reset the control frame callback. the control frame callback hold the 
					// self shared_ptr, if don't reset it, will cause memory leaks.
					this->ws_stream_->control_callback();
				});
			}, chain.move_guard());
		}

		template<typename MatchCondition>
		inline void _ws_post_recv(std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			if (!derive.is_started())
			{
				if (derive.state() == state_t::started)
				{
					derive._do_disconnect(asio2::get_last_error(), std::move(this_ptr));
				}
				return;
			}

			try
			{
				ASIO2_ASSERT(bool(this->ws_stream_));
				// Read a message into our buffer
				this->ws_stream_->async_read(derive.buffer().base(), make_allocator(derive.rallocator(),
				[&derive, this_ptr = std::move(this_ptr), condition = std::move(condition)]
				(const error_code& ec, std::size_t bytes_recvd) mutable
				{
					derive._handle_recv(ec, bytes_recvd, std::move(this_ptr), std::move(condition));
				}));
			}
			catch (system_error & e)
			{
				set_last_error(e);

				derive._do_disconnect(e.code(), derive.selfptr());
			}
		}

		template<typename MatchCondition>
		void _ws_handle_recv(const error_code & ec, std::size_t bytes_recvd,
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			set_last_error(ec);

			if (!derive.is_started())
			{
				if (derive.state() == state_t::started)
				{
					derive._do_disconnect(ec, std::move(this_ptr));
				}
				return;
			}

			// bytes_recvd : The number of bytes in the streambuf's get area up to and including the delimiter.
			if (!ec)
			{
				// every times recv data,we update the last alive time.
				derive.update_alive_time();

				derive._fire_recv(this_ptr, std::string_view(reinterpret_cast<
					std::string_view::const_pointer>(derive.buffer().data().data()), bytes_recvd), condition);

				derive.buffer().consume(bytes_recvd);

				derive._post_recv(std::move(this_ptr), std::move(condition));
			}
			else
			{
				derive._do_disconnect(ec, derive.selfptr());
			}
			// If an error occurs then no new asynchronous operations are started. This
			// means that all shared_ptr references to the connection object will
			// disappear and the object will be destroyed automatically after this
			// handler returns. The connection class's destructor closes the socket.
		}

		template<typename MatchCondition>
		inline void _post_control_callback(
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(bool(this->ws_stream_));
			// Set the control callback. This will be called
			// on every incoming ping, pong, and close frame.
			// can't use push_event, just only use asio::post, beacuse the callback handler
			// will not be called immediately, it is called only when it receives an event 
			// on the another side.
			asio::post(derive.io().context(), make_allocator(derive.wallocator(),
			[this, &derive, this_ptr = std::move(this_ptr), condition = std::move(condition)]() mutable
			{
				this->ws_stream_->control_callback(
				[&derive, this_ptr = std::move(this_ptr), condition = std::move(condition)]
				(websocket::frame_type kind, beast::string_view payload) mutable
				{
					// bug fixed : can't use "std::move(this_ptr)" below, otherwise 
					// when enter this lambda next time, the "this_ptr" is nullptr.
					derive._handle_control_callback(kind, payload, this_ptr, condition);
				});
			}));
		}

		template<typename MatchCondition>
		inline void _handle_control_callback(
			websocket::frame_type kind, beast::string_view payload,
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			detail::ignore_unused(payload, this_ptr, condition);

			// Note that the connection is alive
			derive.update_alive_time();

			switch (kind)
			{
			case websocket::frame_type::ping:
				derive._handle_control_ping(payload, std::move(this_ptr), std::move(condition));
				break;
			case websocket::frame_type::pong:
				derive._handle_control_pong(payload, std::move(this_ptr), std::move(condition));
				break;
			case websocket::frame_type::close:
				derive._handle_control_close(payload, std::move(this_ptr), std::move(condition));
				break;
			default:break;
			}
		}

		template<typename MatchCondition>
		inline void _handle_control_ping(beast::string_view payload,
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			detail::ignore_unused(payload, this_ptr, condition);
		}

		template<typename MatchCondition>
		inline void _handle_control_pong(beast::string_view payload,
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			detail::ignore_unused(payload, this_ptr, condition);
		}

		template<typename MatchCondition>
		inline void _handle_control_close(beast::string_view payload,
			std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			detail::ignore_unused(payload, this_ptr, condition);

			if (derive.state() == state_t::started)
			{
				derive._do_disconnect(websocket::error::closed, std::move(this_ptr));
			}
		}

		template<typename MatchCondition, typename DeferEvent, typename Response, bool IsSession = args_t::is_session>
		typename std::enable_if_t<!IsSession, void>
		inline _post_upgrade(std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition,
			Response& rep, DeferEvent chain)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(bool(this->ws_stream_));

			// Perform the websocket handshake
			this->ws_stream_->async_handshake(rep, derive.host_, derive.get_upgrade_target(),
				make_allocator(derive.wallocator(),
			[&derive, this_ptr = std::move(this_ptr), condition = std::move(condition), chain = std::move(chain)]
			(error_code const& ec) mutable
			{
				derive._handle_upgrade(ec, std::move(this_ptr), std::move(condition), std::move(chain));
			}));
		}

		template<typename MatchCondition, typename DeferEvent, typename Request, bool IsSession = args_t::is_session>
		typename std::enable_if_t<IsSession, void>
		inline _post_upgrade(std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition,
			Request const& req, DeferEvent chain)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(bool(this->ws_stream_));

			// Accept the websocket handshake
			this->ws_stream_->async_accept(req, make_allocator(derive.wallocator(),
			[&derive, this_ptr = std::move(this_ptr), condition = std::move(condition), chain = std::move(chain)]
			(error_code ec) mutable
			{
				derive._handle_upgrade(ec, std::move(this_ptr), std::move(condition), std::move(chain));
			}));
		}

		template<typename MatchCondition, typename DeferEvent, bool IsSession = args_t::is_session>
		typename std::enable_if_t<IsSession, void>
		inline _post_upgrade(std::shared_ptr<derived_t> this_ptr, condition_wrap<MatchCondition> condition,
			DeferEvent chain)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(bool(this->ws_stream_));

			// Accept the websocket handshake
			this->ws_stream_->async_accept(make_allocator(derive.wallocator(),
			[&derive, this_ptr = std::move(this_ptr), condition = std::move(condition), chain = std::move(chain)]
			(error_code ec) mutable
			{
				derive._handle_upgrade(ec, std::move(this_ptr), std::move(condition), std::move(chain));
			}));
		}

		template<typename MatchCondition, typename DeferEvent>
		inline void _handle_upgrade(const error_code & ec, std::shared_ptr<derived_t> this_ptr,
			condition_wrap<MatchCondition> condition, DeferEvent chain)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			ASIO2_ASSERT(bool(this->ws_stream_));

			if constexpr (args_t::is_session)
			{
				// Use "sessions().dispatch" to ensure that the _fire_accept function and the _fire_upgrade
				// function are fired in the same thread
				derive.sessions().dispatch(
				[&derive, ec, this_ptr = std::move(this_ptr), condition = std::move(condition), chain = std::move(chain)]
				() mutable
				{
					try
					{
						set_last_error(ec);

						derive._fire_upgrade(this_ptr);

						asio::detail::throw_error(ec);

						derive._done_connect(ec, std::move(this_ptr), std::move(condition), std::move(chain));
					}
					catch (system_error & e)
					{
						set_last_error(e);

						derive._do_disconnect(e.code(), derive.selfptr(), std::move(chain));
					}
				});
			}
			else
			{
				set_last_error(ec);

				derive._fire_upgrade(this_ptr);

				derive._done_connect(ec, std::move(this_ptr), std::move(condition), std::move(chain));
			}
		}

	protected:
		std::unique_ptr<stream_type>   ws_stream_;
	};
}

#endif // !__ASIO2_WS_STREAM_COMPONENT_HPP__
