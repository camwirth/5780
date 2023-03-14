#ifndef TASK_HPP
#define TASK_HPP
#include <iostream>

class Task {
    public:
        Task(int p, int de, int r, int d, int c, int i) {
            period = p;
            deadline = de;
            releaseTime = r;
            duration = d;
            computationTime = c;
            ID = i;
        }

        Task() {
            period = 0;
            deadline = 0;
            releaseTime = 0;
            duration = 0;
            computationTime = 0;
            ID = 0;
        }
        
        int period;
        int deadline;
        int releaseTime;
        int duration;
        int computationTime;
        char ID;

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

        friend std::ostream& operator << (std::ostream& os, Task& in) {
            os << "Task " << in.ID << " " << std::endl;
        }
};

#endif