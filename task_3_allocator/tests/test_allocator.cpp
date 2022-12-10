#include <string>

#include <boost/test/unit_test.hpp>

#include <block_allocator.hpp>

BOOST_AUTO_TEST_SUITE(test_allocator)

BOOST_AUTO_TEST_CASE(test_init_allocator) {
	allocators::BlockAllocator<int, 4> allocator;
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_allocate_trivial) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	BOOST_CHECK(ptr != nullptr);
	*ptr = 1;
	BOOST_CHECK(*ptr == 1);
	BOOST_CHECK(allocator.size() == 1);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(test_allocate_compound) {
	allocators::BlockAllocator<std::string, 4> allocator;
	auto ptr = allocator.allocate(1);
	BOOST_CHECK(ptr != nullptr);
	*ptr = "hello world";
	BOOST_CHECK(*ptr == std::string("hello world"));
	BOOST_CHECK(allocator.size() == 1);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(test_allocate_muiltiple_elements) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(4);
	BOOST_CHECK(ptr != nullptr);
	for (size_t i = 0; i < 4; i++) {
		ptr[i] = i;
		BOOST_CHECK(ptr[i] == i);
	}
	BOOST_CHECK(allocator.size() == 4);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(test_allocate_muiltiple_times) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 0; i < 4; i++) {
		auto ptr = allocator.allocate(1);
		BOOST_CHECK(ptr != nullptr);
		*ptr = i;
		BOOST_CHECK(*ptr == i);
	}
	BOOST_CHECK(allocator.size() == 4);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(test_allocate_muiltiple_blocks) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 0; i < 5; i++) {
		auto ptr = allocator.allocate(1);
		BOOST_CHECK(ptr != nullptr);
		*ptr = i;
		BOOST_CHECK(*ptr == i);
	}
	BOOST_CHECK(allocator.size() == 5);
	BOOST_CHECK(allocator.blocks_allocated() == 2);
}

BOOST_AUTO_TEST_CASE(test_allocate_with_not_enough_space_in_block) {
	allocators::BlockAllocator<int, 4> allocator;
	allocator.allocate(3);
	allocator.allocate(3);
	BOOST_CHECK(allocator.size() == 6);
	BOOST_CHECK(allocator.blocks_allocated() == 2);
}

BOOST_AUTO_TEST_CASE(test_allocate_more_than_block_size) {
	allocators::BlockAllocator<int, 4> allocator;
	BOOST_CHECK_THROW(allocator.allocate(5), std::bad_alloc);
}

BOOST_AUTO_TEST_CASE(test_allocate_zero) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(0);
	BOOST_CHECK(ptr == nullptr);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_deallocate_one) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.allocate(1);
	allocator.deallocate(ptr, 1);
	BOOST_CHECK(allocator.size() == 1);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(test_deallocate_block) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	*ptr = 1;
	allocator.deallocate(ptr, 1);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_deallocate_many) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(4);
	allocator.deallocate(ptr, 4);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_deallocate_many_blocks) {
	allocators::BlockAllocator<int, 4> allocator;
	int* ptrs[3];
	for (size_t i = 0; i < 3; i++) {
		ptrs[i] = allocator.allocate(4);
		BOOST_CHECK(allocator.size() == (i + 1) * 4);
		BOOST_CHECK(allocator.blocks_allocated() == i + 1);
	}
	for (size_t i = 0; i < 3; i++) {
		allocator.deallocate(ptrs[i], 4);
		BOOST_CHECK(allocator.size() == (2 - i) * 4);
		BOOST_CHECK(allocator.blocks_allocated() == 2 - i);
	}
}

BOOST_AUTO_TEST_CASE(test_deallocate_not_found) {
	allocators::BlockAllocator<int, 4> allocator;
	allocator.allocate(1);
	int* ptr = new int(1);
	BOOST_CHECK_EXCEPTION(
		allocator.deallocate(ptr, 1), std::logic_error,
		[](const auto& ex) {
			return ex.what() == std::string(
				"memory is corrupted or allocated by other allocator"); 
		});
}

BOOST_AUTO_TEST_CASE(test_deallocate_too_much) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	BOOST_CHECK_EXCEPTION(
		allocator.deallocate(ptr, 5), std::logic_error,
		[](const auto& ex) {
			return ex.what() == std::string(
				"memory is corrupted or allocated by other allocator"); 
		});
}

BOOST_AUTO_TEST_CASE(test_reuse_segment_for_new_element) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.allocate(3);
	BOOST_CHECK(allocator.size() == 4);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
	allocator.deallocate(ptr, 1);
	auto new_ptr = allocator.allocate(1);
	BOOST_CHECK(ptr == new_ptr);
	BOOST_CHECK(allocator.size() == 4);
	BOOST_CHECK(allocator.blocks_allocated() == 1);
}

BOOST_AUTO_TEST_CASE(construct) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.construct(ptr, 1);
	BOOST_CHECK(*ptr == 1);
}

BOOST_AUTO_TEST_CASE(construct_compound) {
	allocators::BlockAllocator<std::string, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.construct(ptr, 10, 'c');
	BOOST_CHECK(*ptr == std::string("cccccccccc"));
}

BOOST_AUTO_TEST_CASE(destroy) {
	allocators::BlockAllocator<int, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.construct(ptr, 1);
	allocator.destroy(ptr);
	BOOST_CHECK(*ptr == 1);
}

BOOST_AUTO_TEST_CASE(destroy_compound) {
	struct Destructable {
		~Destructable() { is_desctoyed = true; }
		bool is_desctoyed = false;
	};

	allocators::BlockAllocator<Destructable, 4> allocator;
	auto ptr = allocator.allocate(1);
	allocator.construct(ptr);
	allocator.destroy(ptr);
	BOOST_CHECK(ptr->is_desctoyed);
}

BOOST_AUTO_TEST_CASE(test_clear_empty) {
	allocators::BlockAllocator<int, 4> allocator;
	allocator.clear();
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_clear_data) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 1; i <= 4; i++) {
		allocator.allocate(i);
	}
	allocator.clear();
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_copy_empty) {
	allocators::BlockAllocator<int, 4> allocator;
	allocators::BlockAllocator<int, 4> new_allocator(allocator);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 0);
	BOOST_CHECK(new_allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_copy) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 1; i <= 4; i++) {
		allocator.allocate(i);
	}
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	
	auto new_allocator(allocator);
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	BOOST_CHECK(new_allocator.size() == 10);
	BOOST_CHECK(new_allocator.blocks_allocated() == 3);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment_empty) {
	allocators::BlockAllocator<int, 4> allocator;
	allocators::BlockAllocator<int, 4> new_allocator;
	new_allocator = allocator;
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 0);
	BOOST_CHECK(new_allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 1; i <= 4; i++) {
		allocator.allocate(i);
	}
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	
	allocators::BlockAllocator<int, 4> new_allocator;
	new_allocator = allocator;
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	BOOST_CHECK(new_allocator.size() == 10);
	BOOST_CHECK(new_allocator.blocks_allocated() == 3);
}

BOOST_AUTO_TEST_CASE(test_move_empty) {
	allocators::BlockAllocator<int, 4> allocator;
	allocators::BlockAllocator<int, 4> new_allocator(std::move(allocator));
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 0);
	BOOST_CHECK(new_allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_move) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 1; i <= 4; i++) {
		allocator.allocate(i);
	}
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	
	allocators::BlockAllocator<int, 4> new_allocator(std::move(allocator));
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 10);
	BOOST_CHECK(new_allocator.blocks_allocated() == 3);
}

BOOST_AUTO_TEST_CASE(test_move_assignment_empty) {
	allocators::BlockAllocator<int, 4> allocator;
	allocators::BlockAllocator<int, 4> new_allocator;
	new_allocator = std::move(allocator);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 0);
	BOOST_CHECK(new_allocator.blocks_allocated() == 0);
}

BOOST_AUTO_TEST_CASE(test_move_assignment) {
	allocators::BlockAllocator<int, 4> allocator;
	for (size_t i = 1; i <= 4; i++) {
		allocator.allocate(i);
	}
	BOOST_CHECK(allocator.size() == 10);
	BOOST_CHECK(allocator.blocks_allocated() == 3);
	
	allocators::BlockAllocator<int, 4> new_allocator;
	new_allocator = std::move(allocator);
	BOOST_CHECK(allocator.size() == 0);
	BOOST_CHECK(allocator.blocks_allocated() == 0);
	BOOST_CHECK(new_allocator.size() == 10);
	BOOST_CHECK(new_allocator.blocks_allocated() == 3);
}

BOOST_AUTO_TEST_SUITE_END()
