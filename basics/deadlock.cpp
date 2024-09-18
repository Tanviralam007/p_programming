// deadlock happens when two or more threads wait for each other to release resources, causing the program to freeze
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;

void thread1_f1() {
	std::lock_guard<std::mutex> lock1(mtx1);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::lock_guard<std::mutex> lock2(mtx2);
	std::cout << "thread 1 activity done!\n";
}

void thread2_f2() {
	std::lock_guard<std::mutex> lock1(mtx2);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::lock_guard<std::mutex> lock2(mtx1);
	std::cout << "thread 2 activity done!\n";
}

int main(int argc, char** argv) {

	std::thread thread1(thread1_f1);
	std::thread thread2(thread2_f2);

	thread1.join();
	thread2.join();

	return 0;
}