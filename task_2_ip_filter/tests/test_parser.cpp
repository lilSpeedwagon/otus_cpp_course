#include <sstream>

#include <boost/test/unit_test.hpp>

#include <ip_address.hpp>
#include <parser.hpp>

BOOST_AUTO_TEST_SUITE(test_parser)

BOOST_AUTO_TEST_CASE(test_parse_ip) {
	auto result = parser::ParseAddress("192.168.0.1");
	ip::Address expected{{192, 168, 0, 1}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_parse_invalid_bytes_number) {
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddress("1.1.1.1.1"), std::runtime_error,
		[](const auto& ex) { return ex.what() == std::string("invalid number of address bytes"); });
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddress("1.1.1"), std::runtime_error,
		[](const auto& ex) { return ex.what() == std::string("invalid number of address bytes"); });
}

BOOST_AUTO_TEST_CASE(test_parse_non_numberic) {
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddress("a.b.c.d"), std::runtime_error,
		[](const auto& ex) { return ex.what() == std::string("invalid address byte format"); });
}

BOOST_AUTO_TEST_CASE(test_parse_byte_overflow) {
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddress("255.255.255.256"), std::runtime_error,
		[](const auto& ex) { return ex.what() == std::string("address byte overflow"); });
}

BOOST_AUTO_TEST_CASE(test_parse_ips) {
	std::stringstream ss{"192.168.0.1\thello\tworld\n"
						 "255.255.255.255\tLorem\tIpsum"};
	auto result = parser::ParseAddresses(ss);
	std::vector<ip::Address> expected{{{192, 168, 0, 1}}, {{255, 255, 255, 255}}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_parse_invalid_text) {
	std::stringstream ss{"hello world"};
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddresses(ss), std::runtime_error,
		[](const auto& ex) {
			return ex.what() == std::string("Invalid IP address representation \'hello world\' "
											"on line 1: invalid number of address bytes"); 
		});
}

BOOST_AUTO_TEST_CASE(test_parse_invalid_ips) {
	std::stringstream ss{"192.168.0.1\thello\tworld\n"
						 "255.255.255.256\tLorem\tIpsum"};
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddresses(ss), std::runtime_error,
		[](const auto& ex) {
			return ex.what() == std::string("Invalid IP address representation \'255.255.255.256\' "
											"on line 2: address byte overflow"); 
		});
}

BOOST_AUTO_TEST_CASE(test_parse_missing_byte) {
	std::stringstream ss{"192.168.0.\thello\tworld\n"};
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddresses(ss), std::runtime_error,
		[](const auto& ex) {
			return ex.what() == std::string("Invalid IP address representation \'192.168.0.\' "
											"on line 1: invalid address byte format"); 
		});
}

BOOST_AUTO_TEST_CASE(test_parse_missing_ip) {
	std::stringstream ss{"192.168.0.1\n"
						 "hello\tworld\n"};
	BOOST_CHECK_EXCEPTION(
		parser::ParseAddresses(ss), std::runtime_error,
		[](const auto& ex) {
			return ex.what() == std::string("Invalid IP address representation \'hello\' "
											"on line 2: invalid number of address bytes"); 
		});
}

BOOST_AUTO_TEST_SUITE_END()
