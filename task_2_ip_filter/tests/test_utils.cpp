#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <utils.hpp>

BOOST_AUTO_TEST_SUITE(test_utils)

BOOST_AUTO_TEST_CASE(test_split_empty) {
	auto result = utils::SplitString("", '.');
	std::vector<std::string> expected = {{""}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_split_no_delimiter) {
	auto result = utils::SplitString("abc", '.');
	std::vector<std::string> expected = {{"abc"}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_split_delimiter_only) {
	auto result = utils::SplitString("...", '.');
	std::vector<std::string> expected = {{""}, {""}, {""}, {""}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_split_with_delimiter) {
	auto result = utils::SplitString("a.b.c", '.');
	std::vector<std::string> expected = {{"a"}, {"b"}, {"c"}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(test_split_with_sequental_delimiter) {
	auto result = utils::SplitString("a...bc", '.');
	std::vector<std::string> expected = {{"a"}, {""}, {""}, {"bc"}};
	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_SUITE_END()
