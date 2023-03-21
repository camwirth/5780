#ifndef TASKEDF_H
#define TASKEDF_H

#include <iostream>

class TaskEDF {
    public:
        TaskEDF(int p, int de, int r, int d, int c, char i);
        TaskEDF(); 
        friend inline bool operator < (const TaskEDF& lhs, const TaskEDF& rhs){
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

        friend std::ostream& operator << (std::ostream& os, TaskEDF& in);

        int period;
        int deadline;
        int releaseTime;
        int duration;
        int computationTime;
        char ID;
};

#endif