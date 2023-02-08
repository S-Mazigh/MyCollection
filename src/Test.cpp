#include "MyCollection.hpp"
#include "PrettyPrinting.hpp"
#include <iostream>
#include <random>
#include <cctype>
#include <string>
#include <chrono>
#include <sstream>

using namespace mycollections;

// for pseudo random number generator
// here using creates aliases for the long named types
using u32 = uint_least32_t; // u32 is the smallest datatype supported by the system that can hold at least 32 bits
using engine = std::mt19937;

int main (int argc, char *argv[]) {
    if(argc < 1) {
        std::cerr << "Please give the size of the test collection as a program argument !" << std::endl;
        return 1;
    }
    
    // setting the pseudo random number generator
    std::random_device os_seed; // in linux it uses /dev/random
    const u32 seed = os_seed();

    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(0,1<<16);

    std::vector<u32> testCollection;
    for(int i=0; i<std::stoi(argv[1]); i++) {
        testCollection.emplace_back(distribute(generator));
    }
    
    MyCollection<u32> mytestCollection(testCollection);
    
    
    
    std::stringstream result_print;
    std::chrono::duration<double> elapsed_seconds;
    std::chrono::_V2::system_clock::time_point start, end;
    
    // the same initial collection: sorted in a descending fashion
    mytestCollection.bubbleSort(false);
    std::cout << "Initial Collection: " << mytestCollection << std::endl;
    
    
    // BubbleSort
    prettyPrint::boxedPrint("BubbleSort", 20, 1);
    
    start = std::chrono::system_clock::now();
    
    mytestCollection.bubbleSort();
    mytestCollection.bubbleSort(false);
    
    end = std::chrono::system_clock::now();
    
    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "Time Taken: " << elapsed_seconds.count() << "s";
    
    prettyPrint::boxedPrint(result_print.str());
    
    // SeletionSort
    prettyPrint::boxedPrint("SelectionSort", 20, 1);
    
    start = std::chrono::system_clock::now();
    
    mytestCollection.selectionSort();
    mytestCollection.selectionSort(false);
    
    end = std::chrono::system_clock::now();
    
    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "Time Taken: " << elapsed_seconds.count() << "s";
    
    prettyPrint::boxedPrint(result_print.str());

    // InsertionSort
    prettyPrint::boxedPrint("InsertionSort", 20, 1);
    
    start = std::chrono::system_clock::now();
    
    mytestCollection.insertionSort();
    mytestCollection.insertionSort(false);
    
    end = std::chrono::system_clock::now();
    
    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "Time Taken: " << elapsed_seconds.count() << "s";
    
    prettyPrint::boxedPrint(result_print.str());


    std::cout << "Main end" << std::endl;

    return 0;
}
