#ifndef ASIO2_ENABLE_SSL
#define ASIO2_ENABLE_SSL
#endif

#include <asio2/tcp/tcps_server.hpp>

std::string_view server_key = R"(
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: DES-EDE3-CBC,32DBC28981A12AF1

Ss9aGjqLB5ax69XsLHS7zurjRtGTfUCBOGNVYsFkVnfwjogIwU6cviRoK+T/pB07
lw2Kn/Q98vdoghZilKeH8qE4FTqfeYo0tXx/F8fXzff5UbSKGTVavVFCbphF2Ww4
5iiCoDGi6eY1MRz2eplCWZnHdsGqm9Y6EZ4Pmv/Pg27ZQbFWXPXbvGuC2Ct+rChU
7LnTZzI3UTo4y5RrhtxaBPx/Gx2juOTjIexlzHMA9Bk7q/LdnnxtZQO0cQH+izOC
h7UK01I5hIC5WERobtas2MIhHE2i3YQpm/QjPL5FtckIX2z0NMudHUM1VsYVek8E
Vv1VVIiPApzBpU4S2ov7jtVIu85ayZrgi6m2uAa+R6QJ66uIh1ECD/D4ZCTbDfYO
c+EORUKIU9uQ3VwG028WT8ZyWPxLQAeckd7TYDT4MiEwhnTsnGjpx9caieFcyqtt
SfPAbmzuVLUV+tjVs5msRmZy3s/fUMpA/iFm6zmKz7OoXulHKMG2oVzDkr8jPcSi
6Hgt+xUcsUDj6LKy0xMI3YDC0P/UHly9BL1g/o7wwFBIxnNUYpRIGwMLyUskBROs
hp9gegeOSP/O0LYIoHNi1PFfbiVhV9KaXf+qvLCdgWn9g9APJl/eJZKsP+i124RR
Vk3Cqn8pjDIVh+40yFFuBnECi3NY6000EUKO1PLjjBONOiR5+QTX4qv5gE8pwzlM
SHrfwS58jNw4l9A9rmTLGps4pwhsd4Jlr+OJHj0GeA7q3gC7uJCEjsCE+Cxo/erw
139esKJvgOyJxnMilPfON6u6OIbbKO9IzfR3iIW7oSFr9/7Yh5XOAA==
-----END RSA PRIVATE KEY-----
)";

std::string_view server_crt = R"(
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number: 1 (0x1)
    Signature Algorithm: sha256WithRSAEncryption
        Issuer: C=AU, ST=HENAN, L=ZHENGZHOU, O=WJ, OU=WJSOFT, CN=ZHL/emailAddress=37792738@qq.com
        Validity
            Not Before: Nov 26 08:21:41 2021 GMT
            Not After : Nov 24 08:21:41 2031 GMT
        Subject: C=AU, ST=HENAN, O=WJ, OU=WJSOFT, CN=ZHL/emailAddress=37792738@qq.com
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (1024 bit)
                Modulus:
                    00:c4:ba:4e:5f:22:45:ac:74:8f:5a:c3:06:4b:b4:
                    a6:22:be:68:7b:99:bf:44:02:66:69:09:ec:2c:7a:
                    68:c9:a9:0a:b2:f4:ed:69:6b:ad:29:59:b7:a6:ff:
                    69:df:f6:e5:45:44:d7:70:a7:40:84:d6:19:dd:c4:
                    36:27:86:1d:6d:79:e0:91:e5:77:79:49:28:4f:06:
                    7f:31:70:8b:ec:c2:58:9c:f4:14:1d:29:bb:2c:5a:
                    82:c2:b5:ca:de:eb:cb:a8:34:fc:7b:eb:48:76:44:
                    ed:29:a1:7d:99:3c:ad:a9:3d:8c:8d:ef:12:ef:d5:
                    ad:bf:40:34:b4:fd:e4:f2:a9
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Basic Constraints: 
                CA:FALSE
            Netscape Comment: 
                OpenSSL Generated Certificate
            X509v3 Subject Key Identifier: 
                9B:D5:B6:0E:47:C3:A7:B6:DA:84:3B:F0:CE:D1:50:D3:8F:4F:0A:8A
            X509v3 Authority Key Identifier: 
                keyid:61:74:1F:7E:B1:0E:0D:F9:46:DD:6A:97:85:72:DE:1A:7D:A2:34:65

    Signature Algorithm: sha256WithRSAEncryption
         b6:1e:bb:f7:fa:c5:9f:07:6e:36:9d:2e:7d:39:8e:a1:ed:f1:
         65:a0:0c:e4:bb:6d:bc:eb:58:d5:1d:c2:03:57:8a:41:0a:f1:
         81:0f:87:38:c4:56:83:c3:9d:dc:f3:47:88:c8:a7:ba:69:f9:
         bb:45:1f:73:48:96:f9:d7:fc:da:73:f9:17:5f:2f:94:19:83:
         27:4b:b0:3e:19:29:71:a2:fc:db:d2:5f:6e:4f:e5:f1:d8:35:
         55:f8:d9:db:75:dc:fe:11:e0:9f:70:6e:a8:26:2a:ca:7e:25:
         08:e1:d5:d8:e3:0b:10:48:c6:ae:c5:b4:7b:15:20:87:97:20:
         31:ee:e1:6f:d7:be:41:5d:2a:22:b0:36:16:1d:7a:70:bc:1b:
         d3:89:94:ae:33:66:0c:cd:39:95:9e:69:30:37:05:bb:62:cd:
         3f:dd:2b:bb:72:16:48:75:91:33:33:ae:b7:d7:2d:bd:ce:66:
         f3:6b:69:81:fa:0d:aa:0e:5a:09:9d:24:54:ac:21:9b:14:43:
         44:12:56:8b:cc:13:b5:3b:5a:ba:4e:7b:81:42:1e:38:61:ff:
         a0:a7:01:2f:0b:67:77:90:48:bb:8a:52:62:69:76:3c:a8:a1:
         d6:13:1e:27:f6:02:58:ae:91:4b:9d:37:4e:31:55:73:18:4e:
         d0:61:54:3b
-----BEGIN CERTIFICATE-----
MIIDWDCCAkCgAwIBAgIBATANBgkqhkiG9w0BAQsFADB9MQswCQYDVQQGEwJBVTEO
MAwGA1UECAwFSEVOQU4xEjAQBgNVBAcMCVpIRU5HWkhPVTELMAkGA1UECgwCV0ox
DzANBgNVBAsMBldKU09GVDEMMAoGA1UEAwwDWkhMMR4wHAYJKoZIhvcNAQkBFg8z
Nzc5MjczOEBxcS5jb20wHhcNMjExMTI2MDgyMTQxWhcNMzExMTI0MDgyMTQxWjBp
MQswCQYDVQQGEwJBVTEOMAwGA1UECAwFSEVOQU4xCzAJBgNVBAoMAldKMQ8wDQYD
VQQLDAZXSlNPRlQxDDAKBgNVBAMMA1pITDEeMBwGCSqGSIb3DQEJARYPMzc3OTI3
MzhAcXEuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDEuk5fIkWsdI9a
wwZLtKYivmh7mb9EAmZpCewsemjJqQqy9O1pa60pWbem/2nf9uVFRNdwp0CE1hnd
xDYnhh1teeCR5Xd5SShPBn8xcIvswlic9BQdKbssWoLCtcre68uoNPx760h2RO0p
oX2ZPK2pPYyN7xLv1a2/QDS0/eTyqQIDAQABo3sweTAJBgNVHRMEAjAAMCwGCWCG
SAGG+EIBDQQfFh1PcGVuU1NMIEdlbmVyYXRlZCBDZXJ0aWZpY2F0ZTAdBgNVHQ4E
FgQUm9W2DkfDp7bahDvwztFQ049PCoowHwYDVR0jBBgwFoAUYXQffrEODflG3WqX
hXLeGn2iNGUwDQYJKoZIhvcNAQELBQADggEBALYeu/f6xZ8HbjadLn05jqHt8WWg
DOS7bbzrWNUdwgNXikEK8YEPhzjEVoPDndzzR4jIp7pp+btFH3NIlvnX/Npz+Rdf
L5QZgydLsD4ZKXGi/NvSX25P5fHYNVX42dt13P4R4J9wbqgmKsp+JQjh1djjCxBI
xq7FtHsVIIeXIDHu4W/XvkFdKiKwNhYdenC8G9OJlK4zZgzNOZWeaTA3BbtizT/d
K7tyFkh1kTMzrrfXLb3OZvNraYH6DaoOWgmdJFSsIZsUQ0QSVovME7U7WrpOe4FC
Hjhh/6CnAS8LZ3eQSLuKUmJpdjyoodYTHif2AliukUudN04xVXMYTtBhVDs=
-----END CERTIFICATE-----
)";

std::string_view ca_crt = R"(
-----BEGIN CERTIFICATE-----
MIID0DCCArigAwIBAgIJAKb3L14tWHhjMA0GCSqGSIb3DQEBCwUAMH0xCzAJBgNV
BAYTAkFVMQ4wDAYDVQQIDAVIRU5BTjESMBAGA1UEBwwJWkhFTkdaSE9VMQswCQYD
VQQKDAJXSjEPMA0GA1UECwwGV0pTT0ZUMQwwCgYDVQQDDANaSEwxHjAcBgkqhkiG
9w0BCQEWDzM3NzkyNzM4QHFxLmNvbTAeFw0yMTExMjYwODIxMDlaFw0zMTExMjQw
ODIxMDlaMH0xCzAJBgNVBAYTAkFVMQ4wDAYDVQQIDAVIRU5BTjESMBAGA1UEBwwJ
WkhFTkdaSE9VMQswCQYDVQQKDAJXSjEPMA0GA1UECwwGV0pTT0ZUMQwwCgYDVQQD
DANaSEwxHjAcBgkqhkiG9w0BCQEWDzM3NzkyNzM4QHFxLmNvbTCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBANVbanX0OZCM3X+QbtOpBEBXCmY2IfrTeMNZ
LYr25gPoxFYilPa4CqCZ2hHX2KjvqB04XOYou49g9x6CUsEFxGzr3W1YziL13HQQ
5E2903ntpb2QHChJ1huUoRnsR4cBqn50r1w+a49xffSCbA1QMd1HzdIgwZmH5PQR
epQD3JPKKomSPfS9vLmh7XddfaTX4FsuJXLXr/OON24KYihJ5e18skN4e4h2bViE
0qjWtHeJDomcUyGZmQ3jF4EzBcng8w8XzD5J7lfiOOVMdFt/2Snoe3ylc0vJ6uRv
fLxQquzwVleqZg3v3t4GwKISzQ7wiZi/Gu7wjdzTvQz2mOwRdf0CAwEAAaNTMFEw
HQYDVR0OBBYEFGF0H36xDg35Rt1ql4Vy3hp9ojRlMB8GA1UdIwQYMBaAFGF0H36x
Dg35Rt1ql4Vy3hp9ojRlMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQAD
ggEBAGekedydDeqnWkqh3buU45drHl4I6wOr7AibfNDlNffl8fhG4CjMKKLi1JW/
Z5Nnma/ypeFInKK4LybIsQ8GlLQ144PhGTKsV+9uDKhtiHGzNg5ZGGHqG5NXEjDr
KvqpOlWmaxqIeXlKmGR7a5agaYkd4abHq+Ye/qbZBoiMwJdrpT5LViYLp81pnFt9
62dTNb3vqRWbCgLz4enXsVGHRrWCHT2K9caQeilvVxlpIrvXobKsTfKPKQ5AgzXh
iB3cY4DnXG/RT6LMOU2c5f7SRw2edAMCZMinnkootR/EMlDJgUjSamQ3ssoXqSX3
NcJfvucspIZ1QdOFX5wESIpAMlQ=
-----END CERTIFICATE-----
)";

std::string_view dh = R"(
-----BEGIN DH PARAMETERS-----
MIGHAoGBAPr5rFoiBSxOovqiT+2R06yBd2WcQmVr580kSv/PI2HYdJNNg9JB9/xb
glSjeuKkZPMipdpZygqTk86rrlGWNKcOtybuGSJdcTBMUxcofDTZxIWkwr09JD7c
J5fipXRE8kFry0Nk9lL96seMYoER32zw6y2tXgUeksVrjOkGuheTAgEC
-----END DH PARAMETERS-----
)";

int main()
{
	std::string_view host = "0.0.0.0";
	std::string_view port = "8002";

	bool all_stopped = false;

	asio2::tcps_server server;

	server.set_verify_mode(asio::ssl::verify_peer | asio::ssl::verify_fail_if_no_peer_cert);

	// use memory string for cert
	server.set_cert_buffer(ca_crt, server_crt, server_key, "123456");
	server.set_dh_buffer(dh);

	server.bind_recv([&](auto & session_ptr, std::string_view s)
	{
		printf("recv : %zu %.*s\n", s.size(), (int)s.size(), s.data());

		session_ptr->async_send(s);
	}).bind_accept([&](auto& session_ptr)
	{
		// You can close the connection directly here.
		if (session_ptr->remote_address() == "192.168.0.254")
			session_ptr->stop();

		printf("client accept : %s %u %s %u\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			session_ptr->local_address().c_str(), session_ptr->local_port());
	}).bind_connect([&](auto & session_ptr)
	{
		printf("client enter : %s %u %s %u\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			session_ptr->local_address().c_str(), session_ptr->local_port());
	}).bind_disconnect([&](auto & session_ptr)
	{
		// Used to test that all sessions must be closed before entering the on_stop(bind_stop) function.
		ASIO2_ASSERT(all_stopped == false);

		printf("client leave : %s %u %s\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			asio2::last_error_msg().c_str());
	}).bind_handshake([&](auto & session_ptr)
	{
		if (asio2::get_last_error())
			printf("handshake failure : %d %s\n",
				asio2::last_error_val(), asio2::last_error_msg().c_str());
		else
			printf("handshake success : %s %u\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port());
	}).bind_start([&]()
	{
		if (asio2::get_last_error())
			printf("start tcps server failure : %d %s\n",
				asio2::last_error_val(), asio2::last_error_msg().c_str());
		else
			printf("start tcps server success : %s %u\n",
				server.listen_address().c_str(), server.listen_port());
		//server.stop();
	}).bind_stop([&]()
	{
		all_stopped = true;
		printf("stop tcps server : %d %s\n",
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	});

	server.start(host, port);

	while (std::getchar() != '\n');

	server.stop();

	return 0;
}
