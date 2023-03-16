// read in files
// #include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "priority.hpp"
#include "task.hpp"

int stringToInt(std::string str) {
    int temp = 0;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] >= '0' && str[i] <= '9') {
            temp = temp * 10 + (str[i] - '0');
        }
    }
    return temp;
}

int main(int argc, char** argv) {

    std::vector<Task> periodicTasks;
    std::vector<Task> aperiodicTasks;

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
        input >> numPeriodic >> simulationTime;
        std::cout << "number of periodic tasks: " << numPeriodic << std::endl;
        std::cout << "simulationTime: " << simulationTime << std::endl;
        // input >> simulationTime;

        char ID, comma;
        int computationTime, period, releaseTime;
        std::string line;


        for(int i = 0; i < numPeriodic; i++) {
            input >> ID;
            input >> comma;
            input >> computationTime;
            input >> comma;
            input >> period;

            Task t(period, 0, 0, 0, computationTime, ID);
            periodicTasks.push_back(t);
        }

        int numAperiodic;

        input >> numAperiodic;

        for(int i = 0; i < numAperiodic; i++) {
            input >> ID;
            input >> comma;
            input >> computationTime;
            input >> comma;
            input >> releaseTime;

            Task t(0, 500, releaseTime, computationTime, computationTime, ID);
            aperiodicTasks.push_back(t);
        }

        Priority<Task> queue;

        int clock = 0;
        while(clock != simulationTime) {
            // std::cout << std::endl;
            for(int i = 0; i < periodicTasks.size(); i++) {
                if(clock % periodicTasks[i].period == 0) {
                    std::cout << "Released Task " << periodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                    periodicTasks[i].duration = periodicTasks[i].computationTime;
                    queue.enqueue(periodicTasks[i]);
                }
            }

            for(int i = 0; i < aperiodicTasks.size(); i++) {
                if(clock == aperiodicTasks[i].releaseTime) {
                    std::cout << "Released Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                }
            }

            // std::cout << std::endl;

            if(queue.isEmpty()) {
                // std::cout << "queue is empty. doing work on aperiodic task" << std::endl;
                for(int i = 0; i < aperiodicTasks.size(); i++) {
                    if(clock >= aperiodicTasks[i].releaseTime && aperiodicTasks[i].duration != 0) {
                        std::cout << "\tWork on Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                        aperiodicTasks[i].duration = aperiodicTasks[i].duration - 1;
                        if(aperiodicTasks[i].duration == 0) {
                            std::cout << "Completed Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                        }
                        i = aperiodicTasks.size();
                    }
                }
            }  
            else {
                Task priority = queue.dequeue();
                priority.duration = priority.duration - 1;
                std::cout << "\tWork on Task " << priority.ID << " at time = " << clock << " ms" << std::endl;
                if(priority.duration == 0) {
                    std::cout << "Completed Task " << priority.ID << " at time = " << clock << " ms" << std::endl;
                }
                else {
                    queue.enqueue(priority);
                }
            }

            clock = clock + 1;
                
        }

        input.close();
        output.close();
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