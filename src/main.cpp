#include "main.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <fstream>

namespace bip = boost::interprocess;

std::string Region::getName() {
    return name;
}

uint32_t Region::getSize() {
    return size;
}


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





void computeSharedRegion() {

    namespace bip = boost::interprocess;
    const std::string sharedMemoryName = "FirstMemory";

    //Create or open the shared memory

    bip::shared_memory_object sharedMemory(bip::open_or_create, sharedMemoryName.c_str(), bip::read_write);

    //Set the size of the shared memory

    sharedMemory.truncate(1024);

    //Map the shared memory region into this process's address space

    bip::mapped_region region(sharedMemory, bip::read_write);

    //Get the address of the shared memory

    void* sharedMemoryAddress = region.get_address();

    //Write to the shared memory

    std::strcpy(static_cast<char*>(sharedMemoryAddress), "First messsage");

    // Deallocate and destroy the shared memory

    bip::shared_memory_object::remove(sharedMemoryName.c_str());

}





int main() {

    // Region *r1 = new Region();

    // std::string name1 = r1->getName();
    // uint32_t size1 = r1->getSize();

    // std::clog << name1;
    // std::clog << sizeof(size1);

    computeSharedRegion();

    readSharedMemory();


    std::cout << "CMake build works properly" << std::endl;




    return 0;
}