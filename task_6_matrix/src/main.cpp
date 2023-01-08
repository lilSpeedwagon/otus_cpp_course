#include <iostream>

#include <matrix.hpp>


int main() {
    containers::InfiniteMatrix<int, -1, 2> matrix;
    
    std::cout << matrix[0][0] << '\n';
    std::cout << "size " << matrix.size() << '\n';
    matrix[0][0] = 1;
    std::cout << matrix[0][0] << '\n';
    std::cout << "size " << matrix.size() << '\n';
    matrix[1][1] = 2;
    std::cout << matrix[1][1] << '\n';

    for (const auto& [idx, val] : matrix) {
        for (const auto& i : idx) {
            std::cout << i << ' ';
        }
        std::cout << val << '\n';
    }

    return 0;
}
