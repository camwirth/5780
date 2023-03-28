#ifndef TASK_HPP
#define TASK_HPP
#include <iostream>

class Task {
    public:
        Task(int p, int de, int r, int d, int c, char i);
        Task();

        // overload < operator 
        friend inline bool operator < (const Task& lhs, const Task& rhs) {
            // first priority is based on period of task
            if(lhs.period < rhs.period) {
                return true;
            }
            else if(lhs.period > rhs.period) {
                return false;
            }
            // second priority is based on computation time
            else {
                if(lhs.computationTime <= rhs.computationTime) {
                    return true;
                } else {
                    return false;
                }
            }
        }

        // overload << operator
        friend std::ostream& operator << (std::ostream& os, Task& in);
        
        int period;
        int deadline;
        int releaseTime;
        int duration;
        int computationTime;
        char ID;

};

#endif