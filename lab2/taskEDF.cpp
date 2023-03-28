#include "taskEDF.hpp"

// constructor
TaskEDF::TaskEDF(int p, int de, int r, int d, int c, char i) 
{
    period = p;
    deadline = de;
    releaseTime = r;
    duration = d;
    computationTime = c;
    ID = i;
}

// default constructor
TaskEDF::TaskEDF() 
{
    period = 0;
    deadline = 0;
    releaseTime = 0;
    duration = 0;
    computationTime = 0;
    ID = 0;
}

// << operator to print EDF task
std::ostream& operator << (std::ostream& os, TaskEDF& in) 
{
    os << "Task " << in.ID << " " << std::endl;
}