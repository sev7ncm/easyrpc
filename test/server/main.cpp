#include <iostream>
#include <thread>
#include "easyrpc/EasyRpc.hpp"

int add(int a, int b)
{
    std::cout << "add thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << "add: " << a + b << std::endl;
    return a + b;
}

class Test
{
public:
    int add2(int a, int b)
    {
        std::cout << "add2 thread id: " << std::this_thread::get_id() << std::endl;
        std::cout << "add2: " << a + b << std::endl;
        return a + b;
    }

};

int main()
{
    std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
    Test t;

    std::size_t num = std::thread::hardware_concurrency();
    /* easyrpc::Server server("127.0.0.1", 8888, num); */
    easyrpc::Server server("192.168.244.133", 1234, num);
    try
    {
        server.setThreadPoolSize(2);
        server.bind("add", &add);
        server.bind("add2", &Test::add2, &t);
        server.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Rpc exception: " << e.what() << std::endl;
    }

    std::cout << "Server start..." << std::endl;
    std::cin.get();
    server.stop();
    std::cout << "Server stop..." << std::endl;

    return 0;
}

