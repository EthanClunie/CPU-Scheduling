/**
 * @file schedule_priority_rr.c
 * @author Ethan Clunie
 * @brief Code for the Priority Round Robin process scheduling algorithm
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
    struct node *reader = head, *highest_priority = head, *task_buffer = NULL;

    Task *curr_task = reader->task;

    int buffer_size = 0, tasks_waiting = tid - 1;

    int total_turnaround_time = 0, total_waiting_time = 0;
    float avg_turnaround_time = 0.0, avg_waiting_time = 0.0, avg_response_time = 0.0;

    while (head != NULL)
    {
        reader = head; // Reset reader to head at the start of each iteration
        highest_priority = head;
        while (reader != NULL) /* Get the task with the highest priority */
        {
            if (reader->task->priority > curr_task->priority)
            {
                highest_priority = reader;
            }
            reader = reader->next;
        }
        /* Insert the highest priority task into the task buffer and increment the size of the buffer */
        curr_task = highest_priority->task;
        insert(&task_buffer, curr_task);
        buffer_size++;

        /* Remove the above task from the task list */
        struct node *temp = highest_priority;
        delete(&head, temp->task);

        // reader = head;
        reader = head;
        while (reader != NULL) /* Update the task_buffer by adding any tasks that have the same priority to the curr_task priority */
        {
            if (task_buffer != NULL && task_buffer->task->priority == reader->task->priority)
            {
                curr_task = reader->task;
                insert(&task_buffer, curr_task);
                buffer_size++;

                delete(&head, curr_task);
            }
            reader = reader->next;
        }

        reverseList(&task_buffer); // Reorder the task buffer list to be in order of the tasks that came in first being first in the list

        if(buffer_size == 1) //Only one task in buffer will run normally (standard priority case)
        {
            curr_task = task_buffer->task;
            run(curr_task, curr_task->burst);

            total_turnaround_time += curr_task->burst;
            total_waiting_time += (tasks_waiting) * curr_task->burst;

            struct node *temp = task_buffer;
            delete(&task_buffer, temp->task);
            free(temp);
            tasks_waiting--;
            buffer_size--;
        }
        else // Case for multiple tasks being in the buffer (round-robin case)
        {
            struct node *buffer_reader = task_buffer;
            do
            {
                curr_task = buffer_reader->task;
                if(curr_task->burst >= QUANTUM) /* This branch will run if the current task to run has a remaining burst time greater than QUANTUM */
                {
                    run(curr_task, QUANTUM); //Runs task normally
                    total_waiting_time += QUANTUM * (tasks_waiting); //Adds wait time 10 to every task still in list minus the one running
                    total_turnaround_time += QUANTUM; //Adds 10 to the turnaround execution time
                    curr_task->burst -= QUANTUM; //Decrements the burst time of the task to ensure accurate record keeping of remaining burst time 
                }
                else /* If burst time is less than QUANTUM will run for that amount of time instead of QUANTUM */
                {
                    run(curr_task, curr_task->burst);
                    total_turnaround_time += curr_task->burst;
                    total_waiting_time += curr_task->burst * (tasks_waiting);
                    curr_task->burst -= curr_task->burst;
                }

                if(curr_task->burst == 0)
                {
                    struct node *temp = buffer_reader;
                    buffer_reader = buffer_reader->next;
                    delete(&task_buffer, temp->task);
                    free(temp);
                    tasks_waiting--;
                    buffer_size--;
                }
                else if(buffer_reader != NULL && buffer_reader->next == NULL)
                {
                    buffer_reader = task_buffer; //Resets the reader once reaching the end of the list.
                }
                else
                {
                    buffer_reader = buffer_reader->next;
                }
            } while(buffer_reader != NULL);
        }
        
    }

    /*
    while (loop thru task list so long as it isn't empty) {
        - Set your traversal node to be the head of the list you are traversing. Set the highest priority to be the head node as well

        while (loop thru same list as outer to find the highest priority element) {
            - Find task in list w/ highest priority
            - add task w/ highest prio to runnableTaskBuffer
            - Set current task to be task found above
        }
        - Delete this task from the list *head

        while (loop thru same list to track any tasks in list w/ same priority as current task) {
            - Update runnableTaskBuffer to store references to all tasks w/ same priority
            if (currTask->prio == thisTask->prio) {
                - add to separate list of tasks w/ equal prio
                - set next task to check
                - delete thisTask from list *head
            }
        }

        reverseList()

        if (runnableTaskBuffer->next != NULL) {
            do {
                - Grab task from runnableTaskBuffer
                - Run task for QUANTUM time slice
                - keep doing above 2 until everything in buffer has completed
            } while (runnableTaskBuffer != empty);
        }
        else {
            currTask = *head->task
            run(currTask, currTask->burst)
            - Calculations and shit
        }

        - Reset reader to head of task list
    }
    */

    // Calculate average times
    avg_turnaround_time = (double)total_turnaround_time / tid;
    avg_waiting_time = (double)total_waiting_time / tid;
    avg_response_time = (double)(total_turnaround_time + total_waiting_time) / tid;

    // Print out average times to console
    printf("Average turnaround time: %f units\n", avg_turnaround_time);
    printf("Average waiting time: %f units\n", avg_waiting_time);
    printf("Average response time: %f units\n", avg_response_time);
}