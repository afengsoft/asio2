/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 */

#ifndef __ASIO2_CLIENT_HPP__
#define __ASIO2_CLIENT_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <asio2/base/detail/push_options.hpp>

#include <cstdint>
#include <memory>
#include <chrono>
#include <functional>
#include <atomic>
#include <string>
#include <string_view>
#include <queue>
#include <any>
#include <future>
#include <tuple>
#include <unordered_map>
#include <type_traits>

#include <asio2/external/magic_enum.hpp>

#include <asio2/base/iopool.hpp>
#include <asio2/base/log.hpp>
#include <asio2/base/listener.hpp>
#include <asio2/base/define.hpp>

#include <asio2/base/detail/object.hpp>
#include <asio2/base/detail/allocator.hpp>
#include <asio2/base/detail/util.hpp>
#include <asio2/base/detail/buffer_wrap.hpp>
#include <asio2/base/detail/condition_wrap.hpp>

#include <asio2/base/impl/thread_id_cp.hpp>
#include <asio2/base/impl/connect_time_cp.hpp>
#include <asio2/base/impl/alive_time_cp.hpp>
#include <asio2/base/impl/user_data_cp.hpp>
#include <asio2/base/impl/socket_cp.hpp>
#include <asio2/base/impl/connect_cp.hpp>
#include <asio2/base/impl/disconnect_cp.hpp>
#include <asio2/base/impl/user_timer_cp.hpp>
#include <asio2/base/impl/post_cp.hpp>
#include <asio2/base/impl/connect_timeout_cp.hpp>
#include <asio2/base/impl/event_queue_cp.hpp>
#include <asio2/base/impl/condition_event_cp.hpp>
#include <asio2/base/impl/reconnect_timer_cp.hpp>
#include <asio2/base/impl/send_cp.hpp>

#include <asio2/ecs/rdc/rdc_call_cp.hpp>
#include <asio2/ecs/socks/socks5_client.hpp>

namespace asio2::detail
{
	ASIO2_CLASS_FORWARD_DECLARE_BASE;

	template<class derived_t, class args_t>
	class client_impl_t
		: public object_t              <derived_t        >
		, public iopool_cp             <derived_t, args_t>
		, public thread_id_cp          <derived_t, args_t>
		, public event_queue_cp        <derived_t, args_t>
		, public user_data_cp          <derived_t, args_t>
		, public connect_time_cp       <derived_t, args_t>
		, public alive_time_cp         <derived_t, args_t>
		, public socket_cp             <derived_t, args_t>
		, public connect_cp            <derived_t, args_t>
		, public disconnect_cp         <derived_t, args_t>
		, public reconnect_timer_cp    <derived_t, args_t>
		, public user_timer_cp         <derived_t, args_t>
		, public connect_timeout_cp    <derived_t, args_t>
		, public send_cp               <derived_t, args_t>
		, public post_cp               <derived_t, args_t>
		, public condition_event_cp    <derived_t, args_t>
		, public rdc_call_cp           <derived_t, args_t>
		, public socks5_client_impl    <derived_t, args_t>
	{
		ASIO2_CLASS_FRIEND_DECLARE_BASE;

	public:
		using super = object_t     <derived_t        >;
		using self  = client_impl_t<derived_t, args_t>;

		using iopoolcp = iopool_cp <derived_t, args_t>;

		using args_type   = args_t;
		using key_type    = std::size_t;
		using buffer_type = typename args_t::buffer_t;

		using send_cp<derived_t, args_t>::send;
		using send_cp<derived_t, args_t>::async_send;

	public:
		/**
		 * @brief constructor
		 * @throws maybe throw exception "Too many open files" (exception code : 24)
		 * asio::error::no_descriptors - Too many open files
		 */
		template<class ThreadCountOrScheduler, class ...Args>
		explicit client_impl_t(
			std::size_t init_buf_size,
			std::size_t  max_buf_size,
			ThreadCountOrScheduler&& tcos,
			Args&&...   args
		)
			: super()
			, iopool_cp           <derived_t, args_t>(std::forward<ThreadCountOrScheduler>(tcos))
			, event_queue_cp      <derived_t, args_t>()
			, user_data_cp        <derived_t, args_t>()
			, connect_time_cp     <derived_t, args_t>()
			, alive_time_cp       <derived_t, args_t>()
			, socket_cp           <derived_t, args_t>(iopoolcp::_get_io(0).context(), std::forward<Args>(args)...)
			, connect_cp          <derived_t, args_t>()
			, disconnect_cp       <derived_t, args_t>()
			, reconnect_timer_cp  <derived_t, args_t>()
			, user_timer_cp       <derived_t, args_t>()
			, connect_timeout_cp  <derived_t, args_t>()
			, send_cp             <derived_t, args_t>()
			, post_cp             <derived_t, args_t>()
			, condition_event_cp  <derived_t, args_t>()
			, rdc_call_cp         <derived_t, args_t>()
			, rallocator_()
			, wallocator_()
			, listener_  ()
			, io_        (iopoolcp::_get_io(0))
			, buffer_    (init_buf_size, max_buf_size)
		{
		}

		/**
		 * @brief destructor
		 */
		~client_impl_t()
		{
		}

		/**
		 * @brief start the client
		 */
		inline bool start() noexcept
		{
			ASIO2_ASSERT(this->io_.running_in_this_thread());

			return true;
		}

		/**
		 * @brief async start the client
		 */
		inline bool async_start() noexcept
		{
			ASIO2_ASSERT(this->io_.running_in_this_thread());

			return true;
		}

		/**
		 * @brief stop the client
		 */
		inline void stop()
		{
			ASIO2_ASSERT(this->io_.running_in_this_thread());

			this->derived().dispatch([this]() mutable
			{
				// close reconnect timer
				this->_stop_reconnect_timer();

				// close connect timeout timer
				this->_stop_connect_timeout_timer();

				// close user custom timers
				this->stop_all_timers();

				// close all posted timed tasks
				this->stop_all_timed_tasks();

				// close all async_events
				this->notify_all_condition_events();

				// destroy user data, maybe the user data is self shared_ptr, if don't destroy it,
				// will cause loop refrence.
				this->user_data_.reset();
			});
		}

		/**
		 * @brief check whether the client is started
		 */
		inline bool is_started() const
		{
			return (this->state_ == state_t::started && this->socket_.lowest_layer().is_open());
		}

		/**
		 * @brief check whether the client is stopped
		 */
		inline bool is_stopped() const
		{
			return (this->state_ == state_t::stopped && !this->socket_.lowest_layer().is_open() && this->is_iopool_stopped());
		}

		/**
		 * @brief get this object hash key
		 */
		inline key_type hash_key() const noexcept
		{
			return reinterpret_cast<key_type>(this);
		}

		/**
		 * @brief get the buffer object refrence
		 */
		inline buffer_wrap<buffer_type> & buffer() noexcept { return this->buffer_; }

		/**
		 * @brief get the io object refrence
		 */
		inline io_t & io() noexcept { return this->io_; }

		/**
		 * @brief set the default remote call timeout for rpc/rdc
		 */
		template<class Rep, class Period>
		inline derived_t & set_default_timeout(std::chrono::duration<Rep, Period> duration) noexcept
		{
			this->rc_timeout_ = duration;
			return (this->derived());
		}

		/**
		 * @brief get the default remote call timeout for rpc/rdc
		 */
		inline std::chrono::steady_clock::duration get_default_timeout() noexcept
		{
			return this->rc_timeout_;
		}

	protected:
		/**
		 * @brief get the recv/read allocator object refrence
		 */
		inline auto & rallocator() noexcept { return this->rallocator_; }
		/**
		 * @brief get the send/write allocator object refrence
		 */
		inline auto & wallocator() noexcept { return this->wallocator_; }

		inline listener_t                 & listener() noexcept { return this->listener_; }
		inline std::atomic<state_t>       & state   () noexcept { return this->state_;    }

	public:
		inline constexpr static bool is_session() noexcept { return false; }
		inline constexpr static bool is_client () noexcept { return true ; }
		inline constexpr static bool is_server () noexcept { return false; }
		inline constexpr static bool is_sslmode() noexcept { return false; }

	protected:
		/// The memory to use for handler-based custom memory allocation. used fo recv/read.
		handler_memory<>                            rallocator_;

		/// The memory to use for handler-based custom memory allocation. used fo send/write.
		handler_memory<size_op<>, std::true_type>   wallocator_;

		/// listener 
		listener_t                                  listener_;

		/// The io_context wrapper used to handle the connect/recv/send event.
		io_t                                      & io_;

		/// buffer
		buffer_wrap<buffer_type>                    buffer_;

		/// state
		std::atomic<state_t>                        state_      = state_t::stopped;

		/// Remote call (rpc/rdc) response timeout.
		std::chrono::steady_clock::duration         rc_timeout_ = std::chrono::milliseconds(http_execute_timeout);
	};
}

#include <asio2/base/detail/pop_options.hpp>

#endif // !__ASIO2_CLIENT_HPP__
