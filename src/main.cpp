#include "main.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/thread.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/containers/deque.hpp>

#include <boost/timer/timer.hpp>

#include <vector>
#include <fstream>

namespace bip = boost::interprocess;

std::string Region::getName() {
    return name;
}

uint32_t Region::getSize() {
    return size;
}

// 1

void readSharedMemory() {
    const std::string sharedMemoryName = "SharedMemorytoFile";

    // Create or open the shared memory
    bip::shared_memory_object sharedMemory(bip::open_or_create, sharedMemoryName.c_str(), bip::read_write);

    // Set the size of the shared memory
    sharedMemory.truncate(1024);

    // Map the shared memory region into this process's address space
    bip::mapped_region region(sharedMemory, bip::read_write);

    // Get the address of the shared memory
    void* sharedMemoryAddress = region.get_address();

    // Write to the shared memory 
    std::strcpy(static_cast<char*>(sharedMemoryAddress), "This message will come from a shared memory and it will be written to a txt file \n");

    // Read from the shared memory
    std::string message(static_cast<char*>(sharedMemoryAddress));

    // Append the shared memory content to file

    std::string fileName =  "shared_memory_output.txt";

    std::ofstream outputFile(fileName, std::ios::app); // Open the file in appending mode 

    if (outputFile.is_open()) {
        outputFile << message;
        outputFile.close();
    }

    else {
        std::cerr << "Unable to open the output file!" << std::endl;
        return;
    }

    // Read the file and display its content

    std::ifstream inputFile(fileName);

    if (inputFile.is_open()) {

        std::string fileContent, line;

        // Read each line from the input file and append it to fileContent
        while (std::getline(inputFile, line)) {
            fileContent += line + "\n";  // Append the line and a newline character
        }

        std::cout << "Content from file: " << fileContent << std::endl;
        
        inputFile.close();

    }

    else {
        std::cerr << "Unable to open the input file!" << std::endl;
        return;
    }

    // Deallocate and destroy the shared memory 
    bip::shared_memory_object::remove(sharedMemoryName.c_str());

}


//2

/*
Multithreading Exercise:

Write a program that spawns multiple threads 
using Boost.
Thread and performs a parallel computation task, 
such as calculating the sum of a large array. 
Implement thread synchronization mechanisms, 
such as locks 
or condition variables, to manage access to shared data.
*/

void printMessage(int threadID) {
    std::cout << "Message from thread with ID = " << threadID << std::endl;
}

void multiThreadComputation(uint8_t numThreads) {
    // Container for grouping threads
    boost::thread_group threads;

    // Create multiple threads and assign tasks to each thread to be executed in parallel
    for (int i = 0; i < numThreads; i++) {
        threads.create_thread(boost::bind(&printMessage, i));
    }

    // Wait for all threads to complete the tasks
    threads.join_all();

}


/*

3. Task Distribution Exercise:

Design a program that distributes a 
large dataset into smaller tasks 
and assigns them to multiple threads 
for parallel processing. 
Use techniques like thread pools or 
task queues to efficiently 
manage task distribution and workload balancing.

*/

void processTask(int taskID) {
    std::cout << "Processing task with ID = " << taskID << " in thread " << boost::this_thread::get_id() << std::endl;
}

void distributeTasks(int numTasks, int numThreads) {
    boost::thread_group threads;

    // Create multiple threads and distribute tasks among the threads
    for (int i = 0; i < numThreads; i++) {
        threads.create_thread([&]() {
            for (int j = i; j < numTasks; j++) {
                processTask(j);
            }
        }
        );
    }

    // Wait for all threads which are completing the tasks

    threads.join_all();
}



/*
4. Create a C++ program that 
demonstrates the use of smart pointers 
from Boost.SmartPtr, such as shared_ptr or unique_ptr. 
Implement a scenario where smart pointers are 
used to manage dynamic memory allocation and ensure 
proper memory deallocation without memory leaks.
*/

struct Node {
    int info;
};

void manageNode(Node node) {
    // Shared pointer example
    
    boost::shared_ptr<Node> sharedNodePtr = boost::make_shared<Node>();
    sharedNodePtr -> info = 20;

    // Weak pointer example

    boost::weak_ptr<Node> weakNodePtr = sharedNodePtr;

    // Check if the weak pointer is expired

    if (!weakNodePtr.expired()) {
        // Access the weak pointer if it is still valid

        boost::shared_ptr<Node> sharedPtrFromWeak = weakNodePtr.lock();
        std::cout << "Data from shared pointer: " << sharedPtrFromWeak->info << std::endl; 
    }

    std::cout << node.info << std::endl;
}

/*
5. Concurrent Data Structure Exercise: 
Implement a concurrent queue using 
shared memory and synchronization techniques.

*/

class ConcurrentQueue {
    private:
        interprocess_mutex mtx;
        interprocess_condition cond;
        deque<int> queue;

    public:
        ConcurrentQueue() : mtx(), cond() {}

        void push(int value) {
            boost::lock_guard<bip::interprocess_mutex> lock(mtx);
            queue.push_back(value);
            cond.notify_one();
        }

        int pop() {
            boost::lock_guard<bip::interprocess_mutex> lock(mtx);

            while(queue.empty()) {
                cond.wait(lock);
            }

            int value = queue.front();
            queue.pop_front();

            return value;
        }
};



// Usage example

void useConcurrentDataStructure() {
    ConcurrentQueue queue;
    queue.push(1);
    queue.push(2);

    std::cout << "The popped value is: " << queue.pop() << std::endl;
}

/*
6. Performance Measurement Exercise: 
Develop a program to measure 
execution time 
using Boost.Timer or <chrono> library.
*/

void timeConsumingOperation() {
    // Simulating a time consuming operation
    int arr[100000000];


    int* ptr = arr;

    auto movePointerBy = [](int*& ptr, int i) {
            ptr += i;
    };


    for (int i = 0; i < 10000000; i++) {

        // Lambda expression usage
        movePointerBy(ptr, i);

        std::cout << "Elem at pos" << ptr << " = " << *ptr << std::endl;
    }

}

void measurePerformance() {
    boost::timer::cpu_timer timer;

    // Measure the execution time of the time consuming operation
    timeConsumingOperation();

    boost::timer::cpu_timer elapsedTime = timer.elapsed();
    double seconds = static_cast<double>(elapsedTime.wall) & le-9;

    std::cout << "Elapsed time: " << seconds << " seconds" << std::endl;
}





int main() {

    // Region *r1 = new Region();

    // std::string name1 = r1->getName();
    // uint32_t size1 = r1->getSize();

    // std::clog << name1;
    // std::clog << sizeof(size1);

    //computeSharedRegion();

    //readSharedMemory();


    // multiThreadComputation(7);

    // std::cout << "CMake build works properly" << std::endl;




    return 0;
}