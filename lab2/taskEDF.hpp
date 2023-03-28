#ifndef TASKEDF_H
#define TASKEDF_H

#include <iostream>

class TaskEDF {
    public:
        TaskEDF(int p, int de, int r, int d, int c, char i);
        TaskEDF(); 

        // overload < operator to assert priority 
        friend inline bool operator < (const TaskEDF& lhs, const TaskEDF& rhs){
            // first priority is based on deadline
            if(lhs.deadline < rhs.deadline) {
                return true;
            }
            else if(lhs.deadline > rhs.deadline) {
                return false;
            }
            else {
                // second priority is based on computation time
                if(lhs.computationTime < rhs.computationTime) {
                    return true;
                } else if (lhs.computationTime > rhs.computationTime){
                    return false;
                } 
                // third prioiryt is based on ID name
                else {
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