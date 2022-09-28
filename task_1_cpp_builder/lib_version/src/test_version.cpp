#define BOOST_TEST_MODULE test_version

#include <string>
#include <vector>

#include <version.hpp>

#include <boost/test/unit_test.hpp>

namespace {

std::vector<std::string> split_string(const std::string& str, char delimeter) {
	std::vector<std::string> result{};
	size_t current_token_begin = 0;
	for (;;) {
		size_t current_token_end = str.find(delimeter, current_token_begin);
		if (current_token_end == std::string::npos) {
			break;
		}
		result.push_back(str.substr(current_token_begin,
									current_token_end - current_token_begin));
		current_token_begin = current_token_end + 1;
	}
	result.push_back(str.substr(current_token_begin));
	return result;
}

}

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version) {
	const auto version = version::get_project_version();
	BOOST_CHECK(version.major >= 0);
	BOOST_CHECK(version.minor >= 0);
	BOOST_CHECK(version.build > 0);

	const auto version_str = version.ToString();
	const auto tokens = split_string(version_str, '.');
	BOOST_CHECK(tokens.size() == 3);

	const auto& major_ver_str = tokens.at(0);
	const auto& minor_ver_str = tokens.at(1);
	const auto& build_ver_str = tokens.at(2);

	const auto major_ver = std::stoi(major_ver_str);
	BOOST_CHECK(major_ver == version.major);
	const auto minor_ver = std::stoi(minor_ver_str);
	BOOST_CHECK(minor_ver == version.minor);
	const auto build_ver = std::stoi(build_ver_str);
	BOOST_CHECK(build_ver == version.build);
}

BOOST_AUTO_TEST_SUITE_END()
