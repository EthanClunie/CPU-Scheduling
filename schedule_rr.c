/**
 * @file schedule_rr.c
 * @author Ethan Clunie
 * @brief Code for the Round Robin process scheduling algorithm
 * @version 0.1
 * @date 2024-03-09
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Global list of tasks and task id
struct node *head = NULL;
int tid = 0;

void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(struct task));
    if (newTask == NULL) {
        fprintf(stderr, "Error: Memory allocation for newTask failed in add()");
        exit(EXIT_FAILURE);
    }

    newTask->name = strdup(name);
    newTask->tid = tid;
    newTask->priority = priority;
    newTask->burst = burst;

    insert(&head, newTask);

    __sync_fetch_and_add(&tid, 1);
}

void schedule() {
    reverseList(&head);

    struct node *reader = head;
    Task *curr_task = reader->task;

    int total_turnaround_time = 0, total_waiting_time = 0;
    float avg_turnaround_time = 0.0, avg_waiting_time = 0.0, avg_response_time = 0.0;

    /* Defines an array to hold all of the burst times of all elements in list*/
    int *burst_times = malloc((tid + 1) * sizeof(int));
    /* Defines variable to keep track of list size used instead of tID which has been used as size previously */
    int listSize = 0;
    /* Error handling to make sure array is properly */
    if (burst_times == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory for Round Robin Array.\n");
        exit(EXIT_FAILURE);
    }
    
    struct node *temp = head;
    /* Iterates through and grabs all burst times of tasks to be used in calculations */
    while(temp != NULL)
    {
        burst_times[temp->task->tid] = temp->task->burst;
        temp = temp->next;
        listSize++;
    }
    free(temp);

    while(reader != NULL)
    {
        curr_task = reader->task;
        if(curr_task->burst >= QUANTUM) /* This branch will run if the current task to run has a remaining burst time greater than QUANTUM */
        {
            run(curr_task, QUANTUM); //Runs task normally
            total_waiting_time += QUANTUM * (listSize - 1); //Adds wait time 10 to every task still in list minus the one running
            total_turnaround_time += QUANTUM; //Adds 10 to the turnaround execution time
            curr_task->burst -= QUANTUM; //Decrements the burst time of the task to ensure accurate record keeping of remaining burst time 
        }
        else /* If burst time is less than QUANTUM will run for that amount of time instead of QUANTUM */
        {
            run(curr_task, curr_task->burst);
            total_turnaround_time += curr_task->burst;
            total_waiting_time += curr_task->burst * (listSize - 1);
            curr_task->burst -= curr_task->burst;
        }

        if(curr_task->burst == 0)
        {
            struct node *temp = reader;
            reader = reader->next;
            delete(&head, temp->task);
            free(temp);
            listSize--;
        }
        else if(reader != NULL && reader->next == NULL && listSize > 1)
        {
            reader = head; //Resets the reader once reaching the end of the list.
        }
        else
        {
            reader = reader->next;
        }
    }

    // Calculate average times
    avg_turnaround_time = (double)total_turnaround_time / tid;
    avg_waiting_time = (double)total_waiting_time / tid;
    avg_response_time = (double)(total_turnaround_time + total_waiting_time) / tid;

    // Print out average times to console
    printf("Average turnaround time: %f units\n", avg_turnaround_time);
    printf("Average waiting time: %f units\n", avg_waiting_time);
    printf("Average response time: %f units\n", avg_response_time);
}