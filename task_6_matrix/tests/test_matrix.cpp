#include <sstream>

#include <boost/test/unit_test.hpp>

#include <matrix.hpp>


namespace {

template<typename T, typename N>
std::string UnequalError(const T& lhs, const N& rhs) {
    std::stringstream ss;
    ss << lhs << " != " << rhs;
    return ss.str();
}

} // namespace

BOOST_AUTO_TEST_SUITE(test_matrix_2d)

BOOST_AUTO_TEST_CASE(test_empty_get_elements) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    BOOST_ASSERT(matrix.size() == 0);
    BOOST_ASSERT(matrix.empty());
    for (size_t i = 0; i < 100; i++) {
        for (size_t j = 0; j < 100; j++) {
            const auto val = matrix[i][j];
            BOOST_ASSERT(val == default_value);
        }
    }
    BOOST_ASSERT(matrix.size() == 100 * 100);
}

BOOST_AUTO_TEST_CASE(test_empty_add_element) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[2][1] = 1;
    BOOST_ASSERT(matrix[2][1] == 1);
    BOOST_ASSERT(matrix.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_empty_update_element) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[2][1] = 1;
    matrix[2][1] = 5;
    BOOST_ASSERT(matrix[2][1] == 5);
    BOOST_ASSERT(matrix.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
