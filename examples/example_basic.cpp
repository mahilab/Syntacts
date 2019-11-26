#include <syntacts>
#include <iostream>

#include <typeinfo>

using namespace tact;


struct Data {
    Data() { std::cout << "Data Constructed" << std::endl; }
    ~Data() { std::cout << "Data Destructed" << std::endl; }
    double data;
};

extern HeapPool g_pool;

template<typename T>
struct Deleter {
    void operator()(void* mem) {
        std::cout << "Deleter Called" << std::endl;
        static_cast<T*>(mem)->~T();
        g_pool.deallocate(mem);
    }
};

HeapPool g_pool(sizeof(Data), 256);

int main(int argc, char const *argv[])
{    

    // auto m = g_pool.allocate();
    // g_pool.deallocate(m);
    void* memNew  = new char[sizeof(Data)];
    void* memPool = g_pool.allocate();

    std::cout << g_pool.blocksUsed() << std::endl;

    {
        // auto n = std::unique_ptr<Data, std::default_delete<Data>>(new (memNew) Data());
        auto p = std::unique_ptr<Data,Deleter<Data>>(new (memPool) Data());
    }

    std::cout << g_pool.blocksUsed() << std::endl;


    // std::cout << sizeof(std::unique_ptr<Data,Deleter>) << std::endl;



    // Signal x = Sine(440);
    // Signal y = x;
    // std::cout << x.get() << std::endl;
    // std::cout << y.get() << std::endl;
    // std::cout << Signal::pool().blocksUsed() << std::endl;
    return 0;
}
