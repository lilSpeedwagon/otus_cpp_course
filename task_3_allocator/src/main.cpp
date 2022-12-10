#include <iostream>
#include <map>

#include <block_allocator.hpp>
#include <linked_list.hpp>

int factorial(size_t value) {
    if (value == 0 || value == 1) {
        return 1;
    }
    return factorial(value - 1) * value;
}

int main() {
    constexpr size_t kBlockSize = 10;
    using ComparatorType = std::less<int>;
    using MapAllocatorType = 
        allocators::BlockAllocator<std::map<int, int>::value_type, kBlockSize>;
    using ListAllocatorType = 
        allocators::BlockAllocator<containers::LinkedList<int>::node_type, kBlockSize>;

    // создание экземпляра std::map<int, int>
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    std::map<int, int> base_map;
    for (size_t i = 0; i < kBlockSize; i++) {
        base_map[i] = factorial(i);
    }

    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    std::map<int, int, ComparatorType, MapAllocatorType> custom_allocated_map;
    for (size_t i = 0; i < kBlockSize; i++) {
        custom_allocated_map[i] = factorial(i);
    }
    std::cout << "custom_allocated_map values:\n";
    for (auto [key, value] : custom_allocated_map) {
        std::cout << key << ' ' << value << '\n';
    }

    // создание экземпляра своего контейнера для хранения значений типа int
    // заполнение 10 элементами от 0 до 9
    containers::LinkedList<int> base_list;
    for (size_t i = 0; i < kBlockSize; i++) {
        base_list.push_front(i);
    }

    // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
    // ограниченным 10 элементами
    // заполнение 10 элементами от 0 до 9
    // вывод на экран всех значений, хранящихся в контейнере
    containers::LinkedList<int, ListAllocatorType> custom_allocated_list;
    for (int i = 0; i < 10; i++) {
        custom_allocated_list.push_front(i);
    }
    std::cout << "custom_allocated_list values:\n";
    for (auto value : custom_allocated_list) {
        std::cout << value << '\n';
    }

    return 0;
}

