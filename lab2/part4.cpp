#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "priority.hpp"
#include "task.hpp"
#include "taskEDF.hpp"

int main(int argc, char** argv) 
{
    // create vectors to hold periodic and aperiodic tasks for rm scheduling
    std::vector<Task> periodicTasks;
    std::vector<Task> aperiodicTasks;

    // create vector to hold all tasks for edf scheduling
    std::vector<TaskEDF> edfTasks;
    std::vector<TaskEDF> tasks_in_EDFqueue;

    // create variables to keep track of previous executed task info
    char lastTask;
    int lastTaskdur;

    // create variable for aperiodic tasks' deadline
    int aperiodicDeadline = 500;

    // create variables to keep track of schedule summary
    int RMpreemption = 0;
    int EDFpreemption = 0;
    int RMdeadline_missed = 0;
    int EDFdeadline_missed = 0;
    int RMcompletedTasks = 0;
    int EDFcompletedTasks = 0;
    int RMreleasedTasks = 0;
    int EDFreleasedTasks = 0;

    // check if command line arguments are not valid 
    if(argc != 3) 
    {
        std::cout << "Invalid number of command-line arguments" << std::endl;
        exit(1);
    } 

    // open input file and check if it is valid
    std::ifstream input(argv[1]);
    if(!input.good()) 
    {
        std::cout << "Invalid input file" << std::endl;
        exit(1);
    }

    // open output file and check if it is valid
    std::ofstream output(argv[2]);
    if(!output.good()) 
    {
        std::cout << "Invalid output file" << std::endl;
        exit(1);
    }

    // create variables for number of periodic tasks and simulation time
    int numPeriodic, simulationTime;

    // get number of periodic tasks and simulation time from input file
    input >> numPeriodic >> simulationTime;

    // create variables for file inputs 
    char ID, comma;
    int computationTime, period, releaseTime;

    // loop through periodic tasks to get information
    for(int i = 0; i < numPeriodic; i++) 
    {
        input >> ID;
        input >> comma;
        input >> computationTime;
        input >> comma;
        input >> period;

        // create new instance of the task and add to periodic vector
        Task t(period, 0, 0, 0, computationTime, ID);
        periodicTasks.push_back(t);

        // create new EDF task and add to edf vector
        TaskEDF t2(period, 0, 0, computationTime, computationTime, ID);
        edfTasks.push_back(t2);
    }

    int numAperiodic;

    // check that end of file has not been reached
    if(!input.eof()) 
    {

        // get number of aperiodic tasks
        input >> numAperiodic;

        // loop through aperiodic tasks and read info from file
        for(int i = 0; i < numAperiodic; i++) 
        {
            input >> ID;
            input >> comma;
            input >> computationTime;
            input >> comma;
            input >> releaseTime;

            // create a new instance of the task and add it to aperiodic vector
            Task t(0, releaseTime + aperiodicDeadline, releaseTime, computationTime, computationTime, ID);
            aperiodicTasks.push_back(t);

            // create new EDF task and add to edf vector
            TaskEDF t2(0, releaseTime + aperiodicDeadline, releaseTime, computationTime, computationTime, ID);
            edfTasks.push_back(t2);
        }
    } 
    else 
    {
        // set numAperiodic to 0 since there are no aperiodic tasks
        numAperiodic = 0;
    }

    // print beginning of RM schedule
    output << std::endl << std::endl << "----- RM SCHEDULE -----";
    output << std::endl << std::endl;

    // create priority queue for RM scheduling
    Priority<Task> RMqueue;

    // begin RM simulation 
    int clock = 0;

    // loop until simulation time is reached
    while(clock != simulationTime) 
    {
        
        // loop through all periodic tasks 
        for(int i = 0; i < periodicTasks.size(); i++) 
        {
            // check if any tasks need to be released
            if(clock % periodicTasks[i].period == 0) 
            {
                output << "Task " << periodicTasks[i].ID << " released at ";
                output << clock << " ms" << std::endl;

                // reset duration of task
                periodicTasks[i].duration = periodicTasks[i].computationTime;
                // update deadline
                periodicTasks[i].deadline += periodicTasks[i].period;
                // add the task to the priority queue
                RMqueue.enqueue(periodicTasks[i]);

                // increment released count
                RMreleasedTasks++;
            }
        }

        // loop through all aperiodic tasks
        for(int i = 0; i < aperiodicTasks.size(); i++) 
        {
            // check if any tasks need to be released 
            if(clock == aperiodicTasks[i].releaseTime) 
            {
                output << "Task " << aperiodicTasks[i].ID << " released at ";
                output << clock << " ms" << std::endl;

                // increment released count
                RMreleasedTasks++;
            }
            // check if any tasks have missed their deadline
            if(clock == aperiodicTasks[i].deadline && aperiodicTasks[i].duration != 0)
            {
                // print warning message and discard task
                output << "!!! WARNING !!! Task " << aperiodicTasks[i].ID;
                output << " has missed its deadline and is discarded." << std::endl;

                // update duration to discard task
                aperiodicTasks[i].duration = 0;

                // update missed deadline count
                RMdeadline_missed++;
            }
        }

        // check if the priority queue is empty -- execute aperiodic task
        if(RMqueue.isEmpty()) 
        {   
            // loop through aperiodic tasks
            for(int i = 0; i < aperiodicTasks.size(); i++) 
            {
                // find released aperiodic task that has not been completed
                if(clock >= aperiodicTasks[i].releaseTime && aperiodicTasks[i].duration != 0) 
                {
                    // check for preemption
                    if(lastTask != aperiodicTasks[i].ID && lastTaskdur != 0 && clock != 0) 
                    {
                        // print message
                        output << "--- Task " << lastTask << " preempted by Task ";
                        output << aperiodicTasks[i].ID << " ---" << std::endl;

                        // update preemption count
                        RMpreemption++;
                    } 

                    // check if working on a new task
                    else if(lastTask != aperiodicTasks[i].ID)
                    {
                        output << "\tWorking on Task " << aperiodicTasks[i].ID << " at ";
                        output << clock << " ms" << std::endl;
                    }
                    
                    // do work on task
                    aperiodicTasks[i].duration = aperiodicTasks[i].duration - 1;

                    // update lastTask info
                    lastTask = aperiodicTasks[i].ID;
                    lastTaskdur = aperiodicTasks[i].duration;

                    // check if task is completed
                    if(aperiodicTasks[i].duration == 0) 
                    {
                        // print message
                        output << "Task " << aperiodicTasks[i].ID << " completed at ";
                        output << clock << " ms" << std::endl;

                        // update completed task count
                        RMcompletedTasks++;
                    }

                    // end loop 
                    i = aperiodicTasks.size();
            }
            }
        }  
        else    // priority queue is not empty, work on RM task 
        {
            Priority<Task> temp;

            // loop through queue
            while(!RMqueue.isEmpty())
            {
                // check all tasks in queue
                Task t = RMqueue.dequeue();
                // check for missed deadline
                if(t.deadline == clock)
                {
                    // print error message 
                    output << "!!! WARNING !!! Task " << t.ID;
                    output << " has missed its deadline and is discarded." << std::endl;

                    // update missed deadline count
                    RMdeadline_missed++;
                }
                // enqueue tasks without missed deadlines to temp
                else 
                {
                    temp.enqueue(t);
                }
            }

            // copy temp to RMqueue
            while(!temp.isEmpty())
            {
                Task t = temp.dequeue();
                RMqueue.enqueue(t);
            }

            // dequeue highest priority task
            Task t = RMqueue.dequeue();
            // check for preemption
            if(lastTask != t.ID && lastTaskdur != 0 && clock != 0) 
            {
                // print message
                output << "--- Task " << lastTask << " preempted by Task ";
                output << t.ID << " ---" <<std::endl;

                // update preemption count
                RMpreemption++;
            }
            // check if new task is being worked on
            if(lastTask != t.ID)
            {
                output << "\tWorking on Task " << t.ID << " at ";
                output << clock << " ms" << std::endl;
            }

            // do work to task
            t.duration = t.duration - 1;

            // update lastTask info
            lastTask = t.ID;
            lastTaskdur = t.duration;

            // check if task is completed
            if(t.duration == 0) 
            {
                // print message
                output << "Task " << t.ID << " completed at ";
                output << clock << " ms" << std::endl;

                // update completed task count
                RMcompletedTasks++;
            } 
            else // task is not completed
            {
                // add task back onto priority queue
                RMqueue.enqueue(t);
            }

        }

        // increment clock
        clock = clock + 1;
    }

    // print summary for RM
    output << std::endl << "------ RM SCHEDULE SUMMARY -----" << std::endl;
    output << "Number of released tasks: " << RMreleasedTasks << std::endl;
    output << "Number of preemptions: " << RMpreemption << std::endl;
    output << "Number of completed tasks: " << RMcompletedTasks << std::endl;
    output << "Number of missed deadlines: " << RMdeadline_missed << std::endl;

    // print beginning of EDF schedule
    output << std::endl << std::endl << "----- EDF SCHEDULE -----";
    output << std::endl << std::endl;

    // begin second clock simmulation
    int clock2 = 0;
    // create EDF priority queue
    Priority<TaskEDF> EDFqueue;
    
    // loop through simulation time 
    while(clock2 != simulationTime) 
    {
        // check when tasks need to be released
        for(int i = 0; i < edfTasks.size(); i++) 
        {
            if(clock2 == edfTasks[i].releaseTime) 
            {
                // update release time for periodic tasks
                edfTasks[i].releaseTime = edfTasks[i].releaseTime + edfTasks[i].period;
                // update deadline
                edfTasks[i].deadline += edfTasks[i].period;
                // add task to EDF priority queue
                EDFqueue.enqueue(edfTasks[i]);

                // print message
                output << "Task " << edfTasks[i].ID << " released at ";
                output << clock2 << " ms" << std::endl;

                // increment released count
                EDFreleasedTasks++;
            }
        }

        Priority<TaskEDF> temp;
        int count = 0;
        // loop through queue to check for missed deadlines
        while(!EDFqueue.isEmpty())
        {
            // dequeue each task in queue
            TaskEDF t = EDFqueue.dequeue();

            // check if task has missed deadline
            if(t.deadline == clock2)
            {
                // print error message 
                output << "!!! WARNING !!! Task " << t.ID;
                output << " has missed its deadline and is discarded." << std::endl;

                // update missed deadline count
                EDFdeadline_missed++;
            }
            else 
            {
                // enqueue task to temp if it has not missed the deadline
                temp.enqueue(t);
            }
        }

        // copy temp to taskEDF queue
        while(!temp.isEmpty())
        {
            TaskEDF t = temp.dequeue();
            EDFqueue.enqueue(t);
        }

        // check that queue is not empty
        if(!EDFqueue.isEmpty())
        {    
            // dequeue highest priority task
            TaskEDF t = EDFqueue.dequeue();
            // check for preemption
            if(lastTask != t.ID && lastTaskdur != 0 && clock2 != 0) 
            {
                // print message
                output << "--- Task " << lastTask << " preempted by Task ";
                output << t.ID << " ---" << std::endl;

                // update preemption count
                EDFpreemption++;
            }

            // check if new task is being worked on
            if(lastTask != t.ID) 
            {
                output << "\tWorking on Task " << t.ID << " at ";
                output << clock2 << " ms" << std::endl;
            }

            // do work to task
            t.duration = t.duration - 1;

            // update lastTask info
            lastTask = t.ID;
            lastTaskdur = t.duration;

            // check if task is completed
            if(t.duration == 0) 
            {
                // print message
                output << "Task " << t.ID << " completed at ";
                output << clock2 << " ms" << std::endl;

                // update completed task count
                EDFcompletedTasks++;
            }
            else  // task is not completed
            {
                // add task back onto priority queue
                EDFqueue.enqueue(t);
            }
        }

        // update clock
        clock2++;
    }

    // print summary EDF
    output << std::endl << "------ EDF SCHEDULE SUMMARY -----" << std::endl;
    output << "Number of released tasks: " << EDFreleasedTasks << std::endl;
    output << "Number of preemptions: " << EDFpreemption << std::endl;
    output << "Number of completed tasks: " << EDFcompletedTasks << std::endl;
    output << "Number of missed deadlines: " << EDFdeadline_missed << std::endl;

    // close files
    input.close();
    output.close();

    // end program
    return 0;
}