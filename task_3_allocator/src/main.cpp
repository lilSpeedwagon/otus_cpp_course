#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <block_allocator.hpp>

int main() {
    allocators::BlockAllocator<int, 4> allocator;
    allocator.print();

    auto ptr = allocator.allocate(4);
    allocator.print();
    allocator.deallocate(ptr, 4);
    allocator.print();

    std::cout << std::endl;
    return 0;
}
