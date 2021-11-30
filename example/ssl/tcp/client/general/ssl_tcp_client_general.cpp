#ifndef ASIO2_USE_SSL
#define ASIO2_USE_SSL
#endif

//#include <asio2/asio2.hpp>
#include <asio2/tcp/tcps_client.hpp>
#include <iostream>

std::string_view client_key = R"(
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: DES-EDE3-CBC,967AA0B1DF77C592

/hmvltrtAZ26yCD28FRJWPBvmVwnW3MCt5e56jUcyarCpcRWlB1RRkBd51VQWQ74
9rAtn2HH1nsUMdorSCszcDUkTqLg9DAIiz8qMH0chChNLV173yJTLHJHGjqDnwUY
bQMVJMRjuHBmNfnyoeBbdiYjftpSJVn/G3CZopLdT7Pr+T15Ij8VIuOIrbtEcKBO
VbHMQYgqZeh/7sP2E5RRkddEkFycistiJ+laTP+t3Ro90NyJNy6/JA0tewknFOTK
LpsJ1RoUzTTJWY0QyNDFB5nOahPBIeTrPfzufWxzk6QqXRG4u9NWrCSiJfm/VEet
qQcVVlV7hwsq0NbT5Bqk+Fu632NgN4NPSh2A6noUFUlQm/srebrOFL/2Fsnegc4H
jOJUpbEkTJq9vkdBnLJYXaUS1es07ZzDCL9rqmlLLnkUm+9WuGG2EFwOEz0abUv7
QSyCjqGz1cCe6uvyTZZLKRQhClGNvGk1BZ4pG8PZPqCwi9b1UhTw9FtqWUJKrN3u
w5D81b6d6pZfpSw1Zyvo24GVBZxp9ISO/PN/l32GM1Nv+PRqUHb1Ew83uHuTLFmQ
H+3BBom0gk6k19bESDu0hV4QGmfFASaAb/T31i9Ssd/av4QUJryv+HXp1tiG2W89
eHSVdDVkIf0g80Ryp+VYZhQw9dgywVGxQqs0CLlMMh/BbHnNqc8dLKaVnAiwMEeb
eN7dTsdJVzsOOAtA04IYavQJQAkMbuIGalPg11Fgeb1geB/208xJX8Bf6farT9El
iN1CWBFC/q8QHLbI4DcTV2YCSk19ZV2U+EoDbz1bI9KP61+Rvgb9Rw==
-----END RSA PRIVATE KEY-----
)";

std::string_view client_crt = R"(
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number: 2 (0x2)
    Signature Algorithm: sha256WithRSAEncryption
        Issuer: C=AU, ST=HENAN, L=ZHENGZHOU, O=WJ, OU=WJSOFT, CN=ZHL/emailAddress=37792738@qq.com
        Validity
            Not Before: Nov 26 09:04:13 2021 GMT
            Not After : Nov 24 09:04:13 2031 GMT
        Subject: C=AU, ST=HENAN, O=WJ, OU=WJSOFT, CN=ZHL/emailAddress=37792738@qq.com
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (1024 bit)
                Modulus:
                    00:be:94:6f:5b:ae:20:a8:73:25:3f:a8:4d:92:5a:
                    5b:8b:64:4d:7b:53:2c:fb:10:e9:ad:e5:06:41:5a:
                    f6:eb:58:9a:22:6b:5c:ac:04:03:c5:09:2a:3d:84:
                    d9:34:25:42:76:f8:e6:c7:64:cd:5d:ce:ee:03:54:
                    da:af:dd:da:f2:b4:93:72:3f:26:d6:57:ea:18:ec:
                    9c:c8:20:bc:1a:a1:f9:e0:f5:64:67:9d:61:b8:f6:
                    87:a4:d3:36:01:24:b4:e7:00:c3:54:82:bd:7f:22:
                    48:40:df:43:8c:26:83:aa:b3:68:5d:e9:a1:fe:7c:
                    6f:a6:5d:a4:bd:f4:1f:e3:25
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Basic Constraints: 
                CA:FALSE
            Netscape Comment: 
                OpenSSL Generated Certificate
            X509v3 Subject Key Identifier: 
                50:C7:26:F5:62:F6:B7:24:3C:1C:5C:58:96:08:59:94:A5:7A:A1:22
            X509v3 Authority Key Identifier: 
                keyid:61:74:1F:7E:B1:0E:0D:F9:46:DD:6A:97:85:72:DE:1A:7D:A2:34:65

    Signature Algorithm: sha256WithRSAEncryption
         39:c1:66:e0:1a:68:2a:bc:6e:56:a0:a4:18:53:ac:2e:49:03:
         d3:df:e0:7d:4e:51:4c:0b:fb:f9:1d:ae:a5:b8:16:b1:b6:23:
         db:62:33:25:72:14:99:e1:3a:1a:52:d2:f4:51:74:ef:df:04:
         91:34:4f:e6:57:c2:d6:41:60:05:d5:65:09:6c:44:14:e6:29:
         b8:51:03:e0:bd:33:c3:9d:da:99:24:eb:a8:76:18:88:14:84:
         83:ee:33:a3:5c:05:9a:3c:e3:f0:35:e3:52:a2:4e:aa:39:07:
         41:43:10:3f:cb:03:a2:48:a9:90:ef:5a:53:3b:5b:c7:4d:bb:
         76:05:70:eb:a7:15:22:ad:b5:ed:3b:74:58:71:c0:53:90:44:
         92:81:8b:62:62:2f:3a:96:66:ee:18:53:d4:4c:5e:d6:29:f4:
         4c:18:2c:7a:79:96:38:d7:cf:51:e4:3b:72:9f:9c:be:7f:2e:
         05:8a:06:18:61:62:d5:9c:cc:31:a7:4c:d9:94:bd:d3:b4:22:
         b8:42:d2:6f:99:7a:72:43:b3:a9:03:e2:36:6d:6b:28:4f:f8:
         c5:b5:1b:2b:1d:e9:34:8f:66:0a:13:58:d5:28:38:03:22:bc:
         37:27:ed:c7:b3:c7:80:63:25:d7:fc:38:ad:ac:f9:aa:5b:07:
         15:df:56:17
-----BEGIN CERTIFICATE-----
MIIDWDCCAkCgAwIBAgIBAjANBgkqhkiG9w0BAQsFADB9MQswCQYDVQQGEwJBVTEO
MAwGA1UECAwFSEVOQU4xEjAQBgNVBAcMCVpIRU5HWkhPVTELMAkGA1UECgwCV0ox
DzANBgNVBAsMBldKU09GVDEMMAoGA1UEAwwDWkhMMR4wHAYJKoZIhvcNAQkBFg8z
Nzc5MjczOEBxcS5jb20wHhcNMjExMTI2MDkwNDEzWhcNMzExMTI0MDkwNDEzWjBp
MQswCQYDVQQGEwJBVTEOMAwGA1UECAwFSEVOQU4xCzAJBgNVBAoMAldKMQ8wDQYD
VQQLDAZXSlNPRlQxDDAKBgNVBAMMA1pITDEeMBwGCSqGSIb3DQEJARYPMzc3OTI3
MzhAcXEuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+lG9briCocyU/
qE2SWluLZE17Uyz7EOmt5QZBWvbrWJoia1ysBAPFCSo9hNk0JUJ2+ObHZM1dzu4D
VNqv3drytJNyPybWV+oY7JzIILwaofng9WRnnWG49oek0zYBJLTnAMNUgr1/IkhA
30OMJoOqs2hd6aH+fG+mXaS99B/jJQIDAQABo3sweTAJBgNVHRMEAjAAMCwGCWCG
SAGG+EIBDQQfFh1PcGVuU1NMIEdlbmVyYXRlZCBDZXJ0aWZpY2F0ZTAdBgNVHQ4E
FgQUUMcm9WL2tyQ8HFxYlghZlKV6oSIwHwYDVR0jBBgwFoAUYXQffrEODflG3WqX
hXLeGn2iNGUwDQYJKoZIhvcNAQELBQADggEBADnBZuAaaCq8blagpBhTrC5JA9Pf
4H1OUUwL+/kdrqW4FrG2I9tiMyVyFJnhOhpS0vRRdO/fBJE0T+ZXwtZBYAXVZQls
RBTmKbhRA+C9M8Od2pkk66h2GIgUhIPuM6NcBZo84/A141KiTqo5B0FDED/LA6JI
qZDvWlM7W8dNu3YFcOunFSKtte07dFhxwFOQRJKBi2JiLzqWZu4YU9RMXtYp9EwY
LHp5ljjXz1HkO3KfnL5/LgWKBhhhYtWczDGnTNmUvdO0IrhC0m+ZenJDs6kD4jZt
ayhP+MW1Gysd6TSPZgoTWNUoOAMivDcn7cezx4BjJdf8OK2s+apbBxXfVhc=
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

int main()
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)
	// Detected memory leaks on windows system
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::string_view host = "127.0.0.1";
	std::string_view port = "8002";

	asio2::tcps_client client;

	client.connect_timeout(std::chrono::seconds(10));
	client.auto_reconnect(true, std::chrono::seconds(3));

	client.set_verify_mode(asio::ssl::verify_peer);
	client.set_cert_buffer(ca_crt, client_crt, client_key, "123456");

	client.bind_recv([&](std::string_view sv)
	{
		printf("recv : %u %.*s\n", (unsigned)sv.size(), (int)sv.size(), sv.data());

		client.async_send(sv);

	}).bind_connect([&](asio::error_code ec)
    {
        printf("connect : %d %s\n", ec.value(), ec.message().c_str());

		std::string s;
		s.resize(64);

		client.async_send(s);

    }).bind_handshake([&](asio::error_code ec)
	{
		printf("handshake : %d %s\n", ec.value(), ec.message().c_str());

	}).bind_disconnect([](asio::error_code ec)
	{
		printf("disconnect : %d %s\n", ec.value(), ec.message().c_str());
	});

	if (!client.start(host, port))
	{
		std::cout << "start failed : " << asio2::last_error_msg() << std::endl;
	}
	else
	{
		std::cout << "start success " << std::endl;
	}


	std::string s;
	s += '<';
	int len = 128 + std::rand() % (300);
	for (int i = 0; i < len; i++)
	{
		s += (char)((std::rand() % 26) + 'a');
	}
	s += '>';

	// send data, beacuse may be connect failed,
	// if connect failed, the data will sent failed to.
	//client.async_send(std::move(s), []()
	//{
	//	if (asio2::get_last_error())
	//		std::cout << "send failed : " << asio2::last_error_msg() << std::endl;
	//});

	while (std::getchar() != '\n');

	return 0;
}
