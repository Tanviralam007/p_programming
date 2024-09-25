#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>

std::mutex mtx;

void merge_machine(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left;
    int j = mid + 1;
    int k = 0; // index of temp vec

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::copy(temp.begin(), temp.end(), arr.begin() + left);
    }
}

void threaded_merge_sort(std::vector<int>& arr, int left, int right, int thread_id)
{
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    {
        std::lock_guard<std::mutex> lock1(mtx);
        std::cout << "thread " << thread_id << " started sorting from index " << left << " to " << right << std::endl;
    }

    std::thread t1, t2;
    if (left < mid) {
        t1 = std::thread (threaded_merge_sort, std::ref(arr), left, mid, thread_id * 2);
    }

    if (mid + 1 < right) {
        t2 = std::thread (threaded_merge_sort, std::ref(arr), mid + 1, right, thread_id * 2 + 1);
    }

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();

    merge_machine(arr, left, mid, right);
    
    {
        std::lock_guard<std::mutex> lock2(mtx);
        std::cout << "thread " << thread_id << " finished sorting from index " << left << " to " << right << std::endl;
    }
}

int main(int argc, char** argv) {
    std::vector<int> arr = { 2, 4, 6, 8, 1, 3, 5, 7 };

    std::cout << "non-sorted list-> ";
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl << std::endl;

    int left = 0;
    int right = arr.size() - 1;

    threaded_merge_sort(arr, left, right, 1);

    std::cout << "\nsorted list-> ";
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
}

/*
non-sorted list: 2 4 6 8 1 3 5 7

thread 1 started sorting from index 0 to 7
thread 2 started sorting from index 0 to 3
thread 3 started sorting from index 4 to 7
thread 5 started sorting from index 2 to 3
thread 7 started sorting from index 6 to 7
thread 7 finished sorting from index 6 to 7
thread 4 started sorting from index 0 to 1
thread 6 started sorting from index 4 to 5
thread 6 finished sorting from index 4 to 5
thread 5 finished sorting from index 2 to 3
thread 4 finished sorting from index 0 to 1
thread 3 finished sorting from index 4 to 7
thread 2 finished sorting from index 0 to 3
thread 1 finished sorting from index 0 to 7

sorted list: 1 2 3 4 5 6 7 8
*/