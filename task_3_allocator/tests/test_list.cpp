#include <string>

#include <boost/test/unit_test.hpp>

#include <linked_list.hpp>

BOOST_AUTO_TEST_SUITE(test_list)

BOOST_AUTO_TEST_CASE(test_init) {
	containers::LinkedList<int> list;
	BOOST_CHECK(list.size() == 0);
	BOOST_CHECK(list.empty());
	BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_push_and_iterate) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	BOOST_CHECK(!list.empty());
	BOOST_CHECK(list.size() == 5);
	auto it = list.begin();
	for (int i = 4; i >= 0; i--, it++) {
		BOOST_CHECK(it != list.end());
		BOOST_CHECK(*it == i);
	}
	BOOST_CHECK(++it == list.end());
}

BOOST_AUTO_TEST_CASE(test_clear_empty) {
	containers::LinkedList<int> list;
	list.clear();
	BOOST_CHECK(list.empty());
	BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_clear) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	list.clear();
	BOOST_CHECK(list.empty());
	BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_empty) {
	containers::LinkedList<int> list;
	containers::LinkedList<int> new_list(list);
	BOOST_CHECK(new_list.size() == 0);
	BOOST_CHECK(new_list.empty());
	BOOST_CHECK(new_list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	containers::LinkedList<int> new_list;
	new_list = list;
	BOOST_CHECK(list.size() == new_list.size());
	auto it = list.begin();
	auto new_list_it = new_list.begin();
	for (; it != list.end(); it++, new_list_it++) {
		*it == *new_list_it;
		it != new_list_it;
	}
}

BOOST_AUTO_TEST_CASE(test_copy_assign_empty) {
	containers::LinkedList<int> list;
	containers::LinkedList<int> new_list;
	new_list = list;
	BOOST_CHECK(new_list.size() == 0);
	BOOST_CHECK(new_list.empty());
	BOOST_CHECK(new_list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_assign) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	containers::LinkedList<int> new_list;
	new_list = list;
	BOOST_CHECK(list.size() == new_list.size());
	auto it = list.begin();
	auto new_list_it = new_list.begin();
	for (; it != list.end(); it++, new_list_it++) {
		*it == *new_list_it;
		it != new_list_it;
	}
}

BOOST_AUTO_TEST_CASE(test_move_empty) {
	containers::LinkedList<int> list;
	containers::LinkedList<int> new_list(std::move(list));
	BOOST_CHECK(new_list.size() == 0);
	BOOST_CHECK(new_list.empty());
	BOOST_CHECK(new_list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_move) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	containers::LinkedList<int> new_list;
	new_list = std::move(list);
	BOOST_CHECK(list.empty());
	BOOST_CHECK(new_list.size() == 5);
	auto it = new_list.begin();
	for (int i = 4; i >= 0; i--, it++) {
		BOOST_CHECK(it != new_list.end());
		BOOST_CHECK(*it == i);
	}
}

BOOST_AUTO_TEST_CASE(test_move_assign_empty) {
	containers::LinkedList<int> list;
	containers::LinkedList<int> new_list;
	new_list = std::move(list);
	BOOST_CHECK(new_list.size() == 0);
	BOOST_CHECK(new_list.empty());
	BOOST_CHECK(new_list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_move_assign) {
	containers::LinkedList<int> list;
	for (int i = 0; i < 5; i++) {
		list.push_front(i);
	}
	containers::LinkedList<int> new_list;
	new_list = std::move(list);
	BOOST_CHECK(list.empty());
	BOOST_CHECK(new_list.size() == 5);
	auto it = new_list.begin();
	for (int i = 4; i >= 0; i--, it++) {
		BOOST_CHECK(it != new_list.end());
		BOOST_CHECK(*it == i);
	}
}

BOOST_AUTO_TEST_SUITE_END()
