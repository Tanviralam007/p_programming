#include <iostream>
#include <string>
// Include the thread library
#include <thread>
// Include open mp header file  
#include <omp.h>

int main(int argc, char** argv)
{
    int num_threads = 4;
    int sum = 0;

    // Parallel region with multiple threads
    #pragma omp parallel for reduction(+:sum) num_threads(num_threads)
    for (int i = 0; i < 100; i++) {
        int thread_id = omp_get_thread_num(); // Get thread ID
        // Critical section to prevent threads from printing at the same time
        #pragma omp critical
        {
            std::cout << "Thread " << thread_id << " is processing iteration " << i << std::endl;
        }

        sum = sum + i;
    }

    std::cout << "Total sum: " << sum << std::endl;

    
    return 0;
}
