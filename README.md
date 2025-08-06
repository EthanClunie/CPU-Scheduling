# CPU-Scheduling Project
Repository for my CPU Scheduling project

## Description
The goal with this project is to better familarize myself with the way CPU scheduling is handled and how it can be altered and implemented. To do so, I use some base files as an outline and then implement the actual scheduling algorithms. The supporting files invoke the appropriate scheduling algorithm. These algorithms include: Round Robin, First-Come First-Serve, Shortest Job First, Priority, and Priority Round Robin. The driver.c file will read in given tasks, insert them into a linked list, and invoke the schedule() function to execute the specified scheduling algorithm which is what I must implement. I will be simulate CPU scheduling using the pickNextTask() function to choose the task that should be executed by the CPU using the run() function. For further analysis, the average turnaround time, waiting time, and response time will be calculated for each of the scheduling algorithms. 

## Instructions to run:
 ### Compilation of Scheduling Algorithms into executables -
 - make rr - for round-robin scheduling
 - make fcfs - for FCFS scheduling
 - make sjf - for SJF scheduling
 - make priority - for priority scheduling
 - make priority_rr - for priority with round robin scheduling
 ### Running the Scheduling Algorithms executables -
 - ./rr
 - ./fcfs
 - ./sjf
 - ./priority
 - ./priority_rr

## Source Files:
 ### Given -
 - CPU.c
 - cpu.h
 - driver.c
 - list.c
 - list.h
 - schedulers.h
 - task.h
 - Makefile
 ### Newly Implemented -
 - schedule_fcfs.c
 - schedule_sjf.c
 - schedule_rr.c
 - schedule_priority.c
 - schedule_priority_rr.c

## Submission:
 - schedulers.zip
 
## Example Output:
 - ./priority schedule.txt
 - Running task = [T8] [10] [25] for 25 units.
 - Running task = [T5] [5] [20] for 20 units.
 - Running task = [T4] [5] [15] for 15 units.
 - Running task = [T1] [4] [20] for 20 units.
 - Running task = [T7] [3] [30] for 30 units.
 - Running task = [T3] [3] [25] for 25 units.
 - Running task = [T2] [3] [25] for 25 units.
 - Running task = [T6] [1] [10] for 10 units.
 - Average turnaround time: 21.250000 units
 - Average waiting time: 73.125000 units
 - Average response time: 94.375000 units
