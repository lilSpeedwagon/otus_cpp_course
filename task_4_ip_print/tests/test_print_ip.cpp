#include <sstream>

#include <boost/test/unit_test.hpp>

#include <print_ip.hpp>

namespace {

template<typename T>
void TestAddress(const T& address, const char* expected) {
    std::stringstream ss;
    ip::print_ip(address, ss);
    BOOST_CHECK_MESSAGE(
        strcmp(ss.str().c_str(), expected) == 0,
        "Expected: \"" << expected << "\"; actual: \"" << ss.str() << "\"");
}

} // namespace

BOOST_AUTO_TEST_SUITE(test_print_ip)

BOOST_AUTO_TEST_CASE(test_print_ip_int8) {
    TestAddress(int8_t(0), "0");
    TestAddress(int8_t(1), "1");
    TestAddress(int8_t(127), "127");
    TestAddress(int8_t(-128), "128");
}

BOOST_AUTO_TEST_CASE(test_print_ip_uint8) {
    TestAddress(int8_t(0), "0");
    TestAddress(int8_t(1), "1");
    TestAddress(int8_t(127), "127");
    TestAddress(int8_t(255), "255");
}

BOOST_AUTO_TEST_CASE(test_print_ip_int32) {
    TestAddress(int32_t(0), "0.0.0.0");
    TestAddress(int32_t(256), "0.0.1.0");
    TestAddress(int32_t(2130706433), "127.0.0.1");
    TestAddress(int32_t(-2130706433), "128.255.255.255");
}

BOOST_AUTO_TEST_CASE(test_print_ip_uint32) {
    TestAddress(int32_t(0), "0.0.0.0");
    TestAddress(int32_t(256), "0.0.1.0");
    TestAddress(int32_t(2130706433), "127.0.0.1");
    TestAddress(int32_t(4294967295), "255.255.255.255");
}

BOOST_AUTO_TEST_CASE(test_print_ip_string) {
    TestAddress(std::string(""), "");
    TestAddress(std::string("127.0.0.1"), "127.0.0.1");
    TestAddress(std::string("abc"), "abc");
}

BOOST_AUTO_TEST_CASE(test_print_ip_list) {
    TestAddress(std::list<int>(), "");
    TestAddress(std::list<int>{127, 0, 0, 1}, "127.0.0.1");
    TestAddress(std::list<std::string>{"a", "b", "c"}, "a.b.c");
}

BOOST_AUTO_TEST_CASE(test_print_ip_vector) {
    TestAddress(std::vector<int>(), "");
    TestAddress(std::vector<int>{127, 0, 0, 1}, "127.0.0.1");
    TestAddress(std::vector<std::string>{"a", "b", "c"}, "a.b.c");
}

BOOST_AUTO_TEST_CASE(test_print_ip_tuple) {
    TestAddress(std::make_tuple(), "");
    TestAddress(std::make_tuple(127, 0, 0, 1), "127.0.0.1");
    TestAddress(std::make_tuple("a", "b", "c"), "a.b.c");
}

BOOST_AUTO_TEST_SUITE_END()
