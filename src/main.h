#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>

using namespace boost::interprocess;

/*
Shared Memory Exercise:

Implement a program that creates a shared memory region using Boost.Interprocess 
and allows multiple processes to read and write to it simultaneously. 
Use synchronization mechanisms like mutexes or semaphores to ensure data consistency 
and prevent race conditions.

// shared_memory_object shared_memory{open_or_create, "Reg", read_write};
    // shdmem.truncate(1024);
    // std::cout << shdmem.get_name() << '\n';
    // offset_t size;
    // if (shdmem.get_size(size))
    //     std::cout << size << '\n';

*/

class Region
{
    
    private:
        uint32_t size;  
        std::string name;

    public:
        Region();

        uint32_t getSize();
        std::string getName();

        ~Region();
    
};