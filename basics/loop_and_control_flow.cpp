/*
Exp: Parallelizing a Loop
*/
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
// Include the header thread
#include <thread>

void partial_sum(const std::vector<int>& arr, int start, int end, int& output) {
    output = std::accumulate(arr.begin() + start, arr.begin() + end, 0);
    // std::cout << output << std::endl;
}

int main(int argc, char** argv) {

    int size = 100;
    std::vector<int> arr(size);

    // filling the arr from 1 to size
    std::iota(arr.begin(), arr.end(), 1);

    int result1 = 0, result2 = 0;

    // creating 2 threads
    std::thread thread1(partial_sum, std::ref(arr), 0, size / 2, std::ref(result1));
    std::thread thread2(partial_sum, std::ref(arr), size / 2, size, std::ref(result2));

    thread1.join();
    thread2.join();

    std::cout << "Sum: " << result1 + result2 << std::endl;

    return 0;
}