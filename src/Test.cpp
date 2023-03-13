#include "MyCollection.hpp"
#include "PrettyPrinting.hpp"
#include <iostream>
#include <random>
#include <cctype>
#include <string>
#include <chrono>
#include <sstream>

#define NEW_COLLECTION                                      \
    testCollection.clear();                                 \
    for (int i = 0; i < std::stoi(argv[1]); i++)            \
    {                                                       \
        testCollection.emplace_back(distribute(generator)); \
    }                                                       \
    mytestCollection.setCollection(testCollection);

using namespace mycollections;

// for pseudo random number generator
// here using creates aliases for the long named types
using u32 = uint_least32_t; // u32 is the smallest datatype supported by the system that can hold at least 32 bits
using engine = std::mt19937;

template <typename T>
bool checkingOrder(std::vector<T> const &a, int size, bool ascending);

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        std::cerr << "Please give the size of the test collection as a program argument !" << std::endl;
        return 1;
    }
    // setting the pseudo random number generator
    std::random_device os_seed; // in linux it uses /dev/random
    const u32 seed = os_seed();

    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(0, 1 << 16);

    std::vector<u32> testCollection;
    int initial_size = std::stoi(argv[1]);
    for (int i = 0; i < initial_size; i++)
    {
        testCollection.emplace_back(distribute(generator));
    }

    MyCollection<u32> mytestCollection(testCollection);

    myprint::logger result_print;
    result_print.str(" ");
    result_print << "Current level of logging is " << myprint::logger::CURRENT_LEVEL;
    result_print.log(INFO_LOG);

    std::chrono::duration<double> elapsed_seconds;
    std::chrono::_V2::system_clock::time_point start, end;
    bool orderCheck = false;

    result_print.str(" ");
    result_print << "Initial Collection: " << mytestCollection << std::endl;
    result_print.log(DEBUG_LOG);

    // BubbleSort
    myprint::boxedPrint("BubbleSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.bubbleSort();
    mytestCollection.shamble(); // O(N) rajouté dans le calcul de temps
    mytestCollection.bubbleSort(false);

    end = std::chrono::system_clock::now();

    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";

    myprint::boxedPrint(result_print.str());

    // SeletionSort
    myprint::boxedPrint("SelectionSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.selectionSort();
    mytestCollection.shamble();
    mytestCollection.selectionSort(false);

    end = std::chrono::system_clock::now();
    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";

    myprint::boxedPrint(result_print.str());

    // InsertionSort
    myprint::boxedPrint("InsertionSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.insertionSort();
    mytestCollection.shamble();
    mytestCollection.insertionSort(false);

    end = std::chrono::system_clock::now();

    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";

    myprint::boxedPrint(result_print.str());

    // MergedSort
    myprint::boxedPrint("MergedSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.mergeSortRecursive();
    mytestCollection.shamble();
    mytestCollection.mergeSortRecursive(false);

    end = std::chrono::system_clock::now();

    /*result_print.str(" ");
    result_print << "After Merge Sort: " << mytestCollection << std::endl;
    result_print.log(DEBUG_LOG);*/

    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";
    myprint::boxedPrint(result_print.str());

    // Quicksort
    myprint::boxedPrint("QuickSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.quickSort();
    mytestCollection.shamble();
    mytestCollection.quickSort(false);

    end = std::chrono::system_clock::now();

    /*result_print.str(" ");
    result_print << "After Quick Sort: " << mytestCollection << std::endl;
    result_print.log(DEBUG_LOG);*/

    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";

    myprint::boxedPrint(result_print.str());

    // HeapSort
    myprint::boxedPrint("HeapSort", 20, 1);

    mytestCollection.shamble();

    start = std::chrono::system_clock::now();

    mytestCollection.heapSort();
    mytestCollection.shamble();
    mytestCollection.heapSort(false);

    end = std::chrono::system_clock::now();

    orderCheck = checkingOrder(mytestCollection.getCollection(), initial_size, false);

    result_print.str(" "); // to clear the stream we need to replace the underlying string
    elapsed_seconds = end - start;
    result_print << "(" << orderCheck << ") Time Taken: " << elapsed_seconds.count() << "s";

    myprint::boxedPrint(result_print.str());

    std::cout << "Main end" << std::endl;

    return 0;
}

template <typename T>
bool checkingOrder(std::vector<T> const &a, int size, bool ascending)
{
    if (a.size() != size)
    {
        // std::cout << "Different sizes" << std::endl;
        return false;
    }

    if (ascending)
    {
        for (int i = 0; i < a.size() - 1; i++)
        {
            if (a[i] > a[i + 1])
            {
                std::cout << "Probleme at [" << i << "] where a[" << i << "] = " << a[i] << " and a[" << i + 1 << "] = " << a[i + 1] << std::endl;
                return false;
            }
        }
    }
    else
    {
        for (int i = 0; i < a.size() - 1; i++)
        {
            if (a[i] < a[i + 1])
            {
                std::cout << "Probleme at ["<< i <<"] where a[" << i << "] = " << a[i] << " and a[" << i+1 << "] = " << a[i+1] << std::endl;
                return false;
            }
        }
    }

    return true;
}
