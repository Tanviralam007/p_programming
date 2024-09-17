#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

std::mutex mtx;  
std::map<int, int> counts;

void count_number(const std::vector<int>& arr, int start, int end){
    std::map<int, int> local_counts;
    for(int i = start; i < end; ++i){
        local_counts[arr[i]]++;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        for(const auto& pair : local_counts)
            std::cout << pair.first << " : " << pair.second << std::endl;

        /*
        0 : 5
        1 : 5
        2 : 5
        3 : 5
        4 : 5
        5 : 5
        6 : 5
        7 : 5
        8 : 5
        9 : 5
        0 : 5
        1 : 5
        2 : 5
        3 : 5
        4 : 5
        5 : 5
        6 : 5
        7 : 5
        8 : 5
        9 : 5
        */
    }

    std::lock_guard<std::mutex> lock(mtx);
    for(const auto& pair : local_counts)
        counts[pair.first] = counts[pair.first] + pair.second;
}

int main(int argc, char **argv){
    int size = 100;
    std::vector<int> arr(size);
    for(int i = 0; i < size; i++)
        arr[i] = i % 10;
    
    for(int i = 0; i < 100; ++i)
        std::cout << arr[i] << " ";

    std::thread thread1(count_number, std::ref(arr), 0, size / 2);
    std::thread thread2(count_number, std::ref(arr), size / 2, size);

    thread1.join();
    thread2.join();

    for(const auto& pair : counts)
        std::cout << "Number " << pair.first << "appears " << pair.second << "times" << std::endl;

        /*
        Number 0 appears 10 times
        Number 1 appears 10 times
        Number 2 appears 10 times
        Number 3 appears 10 times
        Number 4 appears 10 times
        Number 5 appears 10 times
        Number 6 appears 10 times
        Number 7 appears 10 times
        Number 8 appears 10 times
        Number 9 appears 10 times
        */
    return 0;
}