// read in files
// #include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "priority.hpp"
#include "task.hpp"
#include "taskEDF.hpp"

int main(int argc, char** argv) 
{

    std::vector<Task> periodicTasks;
    std::vector<Task> aperiodicTasks;

    std::vector<TaskEDF> edfTasks;

    char lastTask;
    int lastTaskdur;

    int aperiodicDeadline = 500;
    int RMpreemption = 0;
    int EDFpreemption = 0;
    int RMdeadline_missed = 0;
    int EDFdeadline_missed = 0;

    // check command line arguments
    if(argc != 3) {
        std::cout << "Invalid number of command-line arguments" << std::endl;
        exit(1);
    } 
    else 
    {
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
        std::cout << "simulation time: " << simulationTime << std::endl;

        char ID, comma;
        int computationTime, period, releaseTime;

        // loop through periodic tasks to get information
        for(int i = 0; i < numPeriodic; i++) {
            input >> ID;
            input >> comma;
            input >> computationTime;
            input >> comma;
            input >> period;

            // create new instance of the task and add to vector
            Task t(period, 0, 0, 0, computationTime, ID);
            periodicTasks.push_back(t);
            TaskEDF t2(period, 0, 0, computationTime, computationTime, ID);
            edfTasks.push_back(t2);
        }

        int numAperiodic;

        // check that end of file has not been reached
        if(!input.eof()) {

            // get number of aperiodic tasks
            input >> numAperiodic;

            // loop through aperiodic tasks and read info from file
            for(int i = 0; i < numAperiodic; i++) {
                input >> ID;
                input >> comma;
                input >> computationTime;
                input >> comma;
                input >> releaseTime;
                std::cout << "ID: " << ID << " release Time: " << releaseTime << std::endl;

                // create a new instance of the task and add it to vector
                Task t(0, aperiodicDeadline, releaseTime, computationTime, computationTime, ID);
                aperiodicTasks.push_back(t);
                TaskEDF t2(0, aperiodicDeadline, releaseTime, computationTime, computationTime, ID);
                edfTasks.push_back(t2);
            }
        } else {
            // set numAperiodic to 0 since there are no aperiodic tasks
            numAperiodic = 0;
        }

        // create priority queue for rm
        Priority<Task> queue;

        // begin RM simulation 
        int clock = 0;

        // find out if a task has been preempted

        // loop until simulation time is reached
        while(clock != simulationTime) {
          
            // loop through all periodic tasks 
            for(int i = 0; i < periodicTasks.size(); i++) {
                // check if any tasks need to be released
                if(clock % periodicTasks[i].period == 0) {
                    std::cout << "Released Task " << periodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                    // set duration of the task to be the computation time
                    periodicTasks[i].duration = periodicTasks[i].computationTime;
                    // update deadline
                    periodicTasks[i].deadline += periodicTasks[i].period;
                    // add the task to the priority queue
                    queue.enqueue(periodicTasks[i]);
                }
            }

            // loop through all aperiodic tasks
            for(int i = 0; i < aperiodicTasks.size(); i++) {
                // check if any tasks need to be released 
                if(clock == aperiodicTasks[i].releaseTime) {
                    std::cout << "Released Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                }
            }

            // check if the priority queue is empty
            if(queue.isEmpty()) {
                
                // loop through aperiodic tasks to do work
                for(int i = 0; i < aperiodicTasks.size(); i++) {
                    // find released aperiodic task that has not been completed
                    if(clock >= aperiodicTasks[i].releaseTime && aperiodicTasks[i].duration != 0) {

                        // do work on task
                        aperiodicTasks[i].duration = aperiodicTasks[i].duration - 1;

                        // check for missed deadline
                        if(aperiodicTasks[i].deadline == clock) {
                            std::cout << "Warning !!! Task " << aperiodicTasks[i].ID << " missed its deadline. This task is discarded." <<std::endl;
                            RMdeadline_missed++;
                            lastTask = aperiodicTasks[i].ID;
                            lastTaskdur = 0;
                        } 
                        else {
                            // check for preemption
                            if(lastTask != aperiodicTasks[i].ID && lastTaskdur != 0 && clock != 0) {
                                std::cout << "Task " << lastTask << " was preempted by Task " << aperiodicTasks[i].ID << std::endl;
                                RMpreemption++;
                            }
                            
                            // update lastTask info
                            lastTask = aperiodicTasks[i].ID;
                            lastTaskdur = aperiodicTasks[i].duration;

                            // check if task is completed
                            if(aperiodicTasks[i].duration == 0) {
                                std::cout << "Completed Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                            }
                        }

                        // std::cout << "\tWork on Task " << aperiodicTasks[i].ID << " at time = " << clock << " ms" << std::endl;
                       
                        // end loop
                        i = aperiodicTasks.size();
                    }
                }
            }  
            else {
                Task priority = queue.dequeue();
                priority.duration = priority.duration - 1;
                if(priority.deadline == clock) {
                    std::cout << "Warning !!! Task " << priority.ID << " missed its deadline. This task is discarded." <<std::endl;
                    RMdeadline_missed++;
                    lastTask = priority.ID;
                    lastTaskdur = 0;
                } else {
                    if(lastTask != priority.ID && lastTaskdur != 0 && clock != 0) {
                        std::cout << "Task " << lastTask << " was preempted by Task " << priority.ID << std::endl;
                        RMpreemption++;
                    }
                    lastTask = priority.ID;
                    lastTaskdur = priority.duration;
                    // std::cout << "\tWork on Task " << priority.ID << " at time = " << clock << " ms" << std::endl;
                    if(priority.duration == 0) {
                        std::cout << "Completed Task " << priority.ID << " at time = " << clock << " ms" << std::endl;
                    }
                    else {
                        queue.enqueue(priority);
                    }
                }
                
            }

            clock = clock + 1;
                
        }

        std::cout << std::endl << "Number of preemptions: " << RMpreemption << " Number of missed deadlines: " << RMdeadline_missed << std::endl << std::endl;

        input.close();
        output.close();

        std::cout << std::endl << std::endl << "-----------------EDF-------------" << std::endl << std::endl;

        for(int i = 0; i < edfTasks.size(); i++) {
            std::cout << "Task " << edfTasks[i].ID << " With releaseTime " << edfTasks[i].releaseTime << std::endl << std::endl;
        }



        int clock2 = 0;
        Priority<TaskEDF> EDFqueue;
        

        while(clock2 != simulationTime) {

            // check when tasks need to be released
            for(int i = 0; i < edfTasks.size(); i++) {
                if(clock2 == edfTasks[i].releaseTime) {
                    // update release time for periodic tasks
                    edfTasks[i].releaseTime = edfTasks[i].releaseTime + edfTasks[i].period;
                    // update deadline
                    edfTasks[i].deadline += edfTasks[i].period;
                    // add task to EDF priority queue
                    EDFqueue.enqueue(edfTasks[i]);
                    std::cout << "Released Task " << edfTasks[i].ID << " at time = " << clock2 << " ms" << std::endl;
                }
            }

            if(!EDFqueue.isEmpty()) {
                TaskEDF t;
                t = EDFqueue.dequeue();
                t.duration = t.duration - 1;

                if(t.ID != lastTask && lastTaskdur != 0 && clock2 != 0) {
                    std::cout << "Task " << lastTask << " was preempted by Task " << t.ID << std::endl;
                    EDFpreemption++;
                }

                if(t.deadline == clock2) {
                    std::cout << "Warning !!! Task " << t.ID << " missed its deadline. This task is discarded." <<std::endl;
                    EDFdeadline_missed++;
                    lastTask = t.ID;
                    lastTaskdur = 0;
                } 
                else 
                {
                    lastTask = t.ID;
                    lastTaskdur = t.duration;

                    // std::cout << "\tWork on Task " << t.ID << " at time = " << clock2 << " ms" << std::endl;
                    if(t.duration == 0) {
                        std::cout << "Completed Task " << t.ID << " at time = " << clock2 << " ms" << std::endl;
                    }

                    else {
                        EDFqueue.enqueue(t);
                    }
                }

                
            }

            clock2++;
            
        }

        std::cout << std::endl << "Number of preemptions: " << EDFpreemption << " Number of missed deadlines: " << EDFdeadline_missed << std::endl << std::endl;
    }

    
}