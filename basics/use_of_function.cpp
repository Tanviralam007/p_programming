#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
// Include the header thread
#include <thread>


// A function for the thread to run
void say_hello_to_thead(const std::string& text);

// Main thread
int main(int argc, char **argv) {
    // Create as much as thread you want, and then start it
    std::thread t1(say_hello_to_thead, "thread_ 1 tuning has started!");
    std::thread t2(say_hello_to_thead, "thread_ 2 tuning has started!");

    // Wait for the thread to finish
    t1.join();
    t2.join();

    std::cout << "Main thread done!" << std::endl;

    return 0;
}

void say_hello_to_thead(const std::string& text) {
    std::cout << text << std::endl;
}