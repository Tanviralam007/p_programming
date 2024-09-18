#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int cnt = 0;

void increment(int thread_id) {
	for (int i = 0; i < 10; ++i) {
		std::lock_guard<std::mutex> lock(mtx);
		cnt = cnt + 1;
		std::cout << "thread id-> " << thread_id << " incremented cnt to " << cnt << std::endl;
	}
}

int main(int argc, char** argv) 
{
	std::thread thread1(increment, 1);
	std::thread thread2(increment, 2);
	std::thread thread3(increment, 3);

	thread1.join();
	thread2.join();
	thread3.join();

	std::cout << "\nshare data value: " << cnt;
}