#ifndef TASKEDF_H
#define TASKEDF_H

#include <iostream>

class TaskEDF {
    public:
        TaskEDF(int p, int de, int r, int d, int c, char i) {
            period = p;
            deadline = de;
            releaseTime = r;
            duration = d;
            computationTime = c;
            ID = i;
        }

        TaskEDF() {
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

        friend inline bool operator < (const TaskEDF& lhs, const TaskEDF& rhs) {
            if(lhs.deadline < rhs.deadline) {
                return true;
            }
            else if(lhs.deadline > rhs.deadline) {
                return false;
            }
            else {
                // maybe do a third condition
                if(lhs.computationTime < rhs.computationTime) {
                    return true;
                } else if (lhs.computationTime > rhs.computationTime){
                    return false;
                } else {
                    return lhs.ID < rhs.ID;
                }
            }
        }

        friend std::ostream& operator << (std::ostream& os, TaskEDF& in) {
            os << "Task " << in.ID << " " << std::endl;
        }
};

#endif