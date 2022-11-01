#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <block_allocator.hpp>

int main() {
    allocators::BlockAllocator<int, 4> allocator;

    auto ptr = allocator.allocate(4);
    allocator.deallocate(ptr, 4);

    return 0;
}
