#include <iostream>

#include <matrix.hpp>


int main() {
    containers::InfiniteMatrix<int, 0, 2> matrix;
    for (int i = 0; i < 10; i++) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "size " << matrix.size() << '\n';

    for (const auto& [index, val] : matrix) {
        std::cout << '(' << index[0] << ';' << index[1] << ')' << ' ' << val << '\n';
    }

    return 0;
}
