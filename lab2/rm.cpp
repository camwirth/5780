// read in files
// #include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(int arc, char** argv) {

    struct {
        char ID;
        int priority, period, computationTime, duration;
        bool overDue;
    } periodicTask

    struct {
        char ID;
        int computationTime, releaseTime, duration;
    } aperiodicTask

    std::vector periodicTasks<periodicTask>;

    // check command line arguments
    if(argc != 3) {
        std::cout << "Invalid number of command-line arguments" << std::endl;
        exit(1);
    } 
    else {
        // open input file and check if it is valid
        std::ifstream input(argv[1]);
        if(!input.good()) {
            std::cout << "Invalid input file" << std::endl;
            exit(1);
        }

        // open output file and check if it is valid
        std::ofstream output(argv[2]);
        if(!output.good()) {
            std::cout << "Invalid output file" << std::endl;
            exit(1);
        }

        int numPeriodic, simulationTime;

        // get number of periodic tasks and simulation time from input file
        input >> numPeriodic;
        input >> simulationTime;

        for(int i = 0; i < numPeriodic; i++) {
            periodicTask task;

            input >> task.ID >> task.computationTime >> task.period;
            periodicTask.add(task);

            // add instance of task to a vector
        }

        int numAperiodic;

        input >> numAperiodic;

        for(int i = 0; i < numAperiodic; i++) {
            char TaskId;
            int computationTime, releaseTime;

            input >> TaskId >> computationTime >> period;

            // create instance of aperiodic task

            // add instance of task to a vector
        }

    }
}

// create new tasks and assign priorities (sort)
    // first based off of period (shorter higher priority)
    // compare computation or execution time (longer higher priority)

// begin clock

// at each clock cycle
    // check release times and update priority queue
    // pop off highest priority task
    // do work
    // check if task is done
        // if not put back onto priority queue
    // check if deadline has been missed