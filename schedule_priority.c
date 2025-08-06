/**
 * @file schedule_priority.c
 * @author Ethan Clunie
 * @brief Code for the Priority process scheduling algorithm
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
    struct node *reader = head, *highest_priority = head;
    Task *curr_task = reader->task;

    int total_turnaround_time = 0, total_waiting_time = 0;
    float avg_turnaround_time = 0.0, avg_waiting_time = 0.0, avg_response_time = 0.0;

    while (head != NULL)
    {
        reader = head; // Reset reader to head at the start of each iteration
        highest_priority = head;
        curr_task = highest_priority->task;
        while (reader != NULL)
        {
            if (reader->task->priority > curr_task->priority)
            {
                highest_priority = reader;
                curr_task = highest_priority->task;
            }
            reader = reader->next;
        }
        run(curr_task, curr_task->burst);

        total_turnaround_time += curr_task->burst;
        total_waiting_time += (tid - (curr_task->tid + 1)) * curr_task->burst;

        struct node *temp = highest_priority;
        delete(&head, temp->task);
        free(temp);
        highest_priority = head;
        reader = head;
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