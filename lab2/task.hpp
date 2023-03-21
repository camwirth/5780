#ifndef TASK_HPP
#define TASK_HPP
#include <iostream>

class Task {
    public:
        Task(int p, int de, int r, int d, int c, char i);
        Task();

        friend inline bool operator < (const Task& lhs, const Task& rhs) {
            if(lhs.period < rhs.period) {
                return true;
            }
            else if(lhs.period > rhs.period) {
                return false;
            }
            else {
                if(lhs.computationTime <= rhs.computationTime) {
                    return true;
                } else {
                    return false;
                }
            }
        }

        friend std::ostream& operator << (std::ostream& os, Task& in);
        
        int period;
        int deadline;
        int releaseTime;
        int duration;
        int computationTime;
        char ID;

};

#endif