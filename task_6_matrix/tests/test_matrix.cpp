#include <boost/test/unit_test.hpp>

#include <matrix.hpp>


BOOST_AUTO_TEST_SUITE(test_matrix_2d)

BOOST_AUTO_TEST_CASE(test_empty_get_default_elements) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    BOOST_ASSERT(matrix.size() == 0);
    BOOST_ASSERT(matrix.empty());
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            const auto val = matrix[i][j];
            BOOST_ASSERT(val == default_value);
        }
    }
    BOOST_ASSERT(matrix.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_add_element) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[2][1] = 1;
    BOOST_ASSERT(matrix[2][1] == 1);
    BOOST_ASSERT(matrix.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_empty_add_elements) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    for (size_t i = 0; i < 10; i++) {
        matrix[i][i] = i;
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            BOOST_ASSERT(matrix[i][j] == (i == j ? i : default_value));
        }
    }
    BOOST_ASSERT(matrix.size() == 10);
}

BOOST_AUTO_TEST_CASE(test_empty_update_element) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[2][1] = 1;
    matrix[2][1] = 5;
    BOOST_ASSERT(matrix[2][1] == 5);
    BOOST_ASSERT(matrix.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_empty_set_to_default) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[0][1] = 1;
    matrix[0][1] = default_value;
    BOOST_ASSERT(matrix[2][1] == default_value);
    BOOST_ASSERT(matrix.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_copy_iterator) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[0][0] = 2;
    auto it = matrix[0];
    auto it2 = it;
    BOOST_ASSERT(it2[0] == 2);
    it2[0] = 3;
    BOOST_ASSERT(matrix[0][0] == 3);
}

BOOST_AUTO_TEST_CASE(test_empty_copy_proxy) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    matrix[0][0] = 2;
    auto proxy = matrix[0][0];
    auto proxy2 = proxy;
    BOOST_ASSERT(proxy2 == 2);
    proxy2 = 3;
    BOOST_ASSERT(matrix[0][0] == 3);
}

BOOST_AUTO_TEST_CASE(test_invalidated_iterator) {
    constexpr int default_value = -1;
    
    std::optional<containers::InfiniteMatrix<int, default_value, 2>::IteratorProxy<1, 1>> it_opt = std::nullopt;
    {
        containers::InfiniteMatrix<int, default_value, 2> matrix;
        it_opt = matrix[0];
    }
    BOOST_CHECK_THROW(it_opt.value()[0], std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_invalidated_proxy) {
    constexpr int default_value = -1;
    
    std::optional<containers::InfiniteMatrix<int, default_value, 2>::ValueProxy> proxy_opt = std::nullopt;
    {
        containers::InfiniteMatrix<int, default_value, 2> matrix;
        proxy_opt = matrix[0][1];
    }
    BOOST_CHECK_THROW(proxy_opt.value() = 1, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_enumerate) {
    constexpr int default_value = -1;
    containers::InfiniteMatrix<int, default_value, 2> matrix;
    std::unordered_map<std::array<size_t, 2>, int> expected;
    for (size_t i = 0; i < 10; i++) {
        matrix[i][i] = i;
        expected[std::array<size_t, 2>{i, i}] = i;
    }

    for (const auto& [index, val] : matrix) {
        auto it = expected.find(index);
        BOOST_ASSERT(it != expected.end());
        BOOST_ASSERT(it->first == index);
        BOOST_ASSERT(it->second == val);
    }
    BOOST_ASSERT(matrix.size() == 10);
}

BOOST_AUTO_TEST_SUITE_END()
