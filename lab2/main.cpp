#include "task.hpp"
#include "priority.hpp"
#include <vector>
#include <iostream>

int main(void) {

    std::cout << "Creating task A with period 100, duration 10" << std::endl;
    Task t1(100, 100, 0, 10, 10, 'A');
    
    std::cout << "Creating task B with period 100, duration 15" << std::endl;
    Task t2(100, 100, 0, 15, 15, 'B');

    std::cout << "Creating task C with period 200, duration 20" << std::endl;
    Task t3(200, 200, 0, 20, 20, 'C');

    Priority<Task> queue;

    queue.enqueue(t1);

    queue.printQueue();

    queue.enqueue(t2);

    queue.printQueue();

    queue.enqueue(t3);

    queue.printQueue();

    Task temp; 

    temp = queue.dequeue();
    std::cout << temp;

    temp = queue.dequeue();
    std::cout << temp;

    temp = queue.dequeue();
    std::cout << temp;

    queue.dequeue();
}