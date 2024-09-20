#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>

std::mutex mtx;

void threaded_quick_sort(std::vector<int>& arr, int left, int right, int thread_id) {
    if (left >= right)
        return;

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << thread_id << " started sorting from index "
            << left << " to " << right << std::endl;
    }

    int pivot = arr[left + (right - left) / 2];
    int i = left;
    int j = right;

    while (i <= j) {
        while (arr[i] < pivot) 
            ++i;
        while (arr[j] > pivot)
            --j;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    // Create new threads for left and right partitions
    if (left < j) {
        std::thread left_thread(threaded_quick_sort, std::ref(arr), left, j, thread_id * 2);
        left_thread.detach();
    }

    if (i < right) {
        threaded_quick_sort(arr, i, right, thread_id * 2 + 1);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << thread_id << " finished sorting from index "
            << left << " to " << right << std::endl;
    }
}

int main(int argc, char** argv) {
    std::vector<int> arr = { 9, 7, 5, 11, 12, 2, 14, 3, 10, 6 };

    threaded_quick_sort(arr, 0, static_cast<int>(arr.size()) - 1, 1);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\nSorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}

/*
Expected Output: 

Thread 1 started sorting from index 0 to 9
Thread 3 started sorting from index 8 to 9
Thread 3 finished sorting from index 8 to 9
Thread 1 finished sorting from index 0 to 9
Thread 2 started sorting from index 0 to 7
Thread 2 finished sorting from index 0 to 7
Thread 4 started sorting from index 0 to 6
Thread 9 started sorting from index 2 to 6
Thread 8 started sorting from index 0 to 1
Thread 8 finished sorting from index 0 to 1
Thread 18 started sorting from index 2 to 3
Thread 18 finished sorting from index 2 to 3
Thread 19 started sorting from index 4 to 6
Thread 19 finished sorting from index 4 to 6
Thread 9 finished sorting from index 2 to 6
Thread 4 finished sorting from index 0 to 6

Sorted array: 2 3 5 6 7 9 10 11 12 14

*/