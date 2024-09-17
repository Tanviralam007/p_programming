#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
/*
mutex - also known as 'Mutual exclusion'
how does it work?
    -> it locks a specific resource when a thread wants to access it
    -> if another thread wants to access the same resource, it has to wait until the current thread activity is done,
       which is called Releasing Mutex
    -> Once the first thread activity is done, it unlocks the mutex, allowing the other thread to use the resources
*/
int cnt = 0;
void salary_increment(){
    for(int i = 0; i < 10; i++){
        mtx.lock();

        cnt = cnt + 1;

        mtx.unlock();
    }
}

int main(int argc, char **argv){
    std::thread thread1(salary_increment);
    std::thread thread2(salary_increment);

    thread1.join();
    /*1 2 3 4 5 6 7 8 8 9 10*/
    thread2.join();
    /*11 12 13 14 15 16 17 18 19 20*/ 

    std::cout << "Final Count: " << cnt << std::endl; // Expected output : 20

    return 0;
}