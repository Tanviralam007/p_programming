#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex coutMutex;  

const int SIZE = 4;

void multiplyMatrix(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C, int startRow, int endRow, int threadID) {
        {
            std::lock_guard<std::mutex> lock(coutMutex);  
            std::cout << "Thread " << threadID << " starting. Processing rows from " << startRow << " to " << endRow - 1 << std::endl;
        }

        for (int i = startRow; i < endRow; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                C[i][j] = 0;
                for (int k = 0; k < SIZE; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            {
                std::lock_guard<std::mutex> lock(coutMutex);  
                std::cout << "Thread " << threadID << " finished row " << i << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(coutMutex);  
            std::cout << "Thread " << threadID << " finished its activity." << std::endl;
        }
}

void print_matrix(std::vector<std::vector<int>>& matrix) {
    for (const auto& rows : matrix) {
        for (int elements : rows) {
            std::cout << elements << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<std::vector<int>> A(SIZE, std::vector<int>(SIZE, 1));  
    std::vector<std::vector<int>> B(SIZE, std::vector<int>(SIZE, 2));  
    std::vector<std::vector<int>> C(SIZE, std::vector<int>(SIZE, 0));  
    
    std::cout << "Matrix A: " << std::endl;
    print_matrix(A);
    std::cout << std::endl;

    std::cout << "Matrix B: " << std::endl;
    print_matrix(B);
    
    int numThreads = 2;  // Using 2 threads
    int rowsPerThread = SIZE / numThreads;

    std::vector<std::thread> threads;

    // Create and launch threads
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? SIZE : startRow + rowsPerThread;
        threads.emplace_back(multiplyMatrix, std::ref(A), std::ref(B), std::ref(C), startRow, endRow, i + 1);  
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Matrix C:\n";
    print_matrix(C);

    
}

/*
Expected Output: 
Matrix A:
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1

Matrix B:
2 2 2 2
2 2 2 2
2 2 2 2
2 2 2 2

Thread 2 starting. Processing rows from 2 to 3
Thread 1 starting. Processing rows from 0 to 1
Thread 2 finished row 2
Thread 1 finished row 0
Thread 2 finished row 3
Thread 2 finished its activity.
Thread 1 finished row 1
Thread 1 finished its activity.

Matrix C:
8 8 8 8
8 8 8 8
8 8 8 8
8 8 8 8

*/