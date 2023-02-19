#include <iostream>
#include <thread>

#include <async.hpp>


int main() {
    std::size_t block_size = 5;
    auto h = async::Connect(block_size);
    auto h2 = async::Connect(block_size);
    async::Receive(h, "1", 1);
    async::Receive(h2, "1\n", 2);
    async::Receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::Receive(h, "b\nc\nd\n}\n89\n", 11);
    async::Disconnect(h);
    async::Disconnect(h2);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
