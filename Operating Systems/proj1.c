#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 26
#define MAX_QUEUES 9

typedef struct {
    char id;
    int arrival_time;
    int burst_time;
    int time_spent;
    int queue_level;
    int response_time;
    int turnaround_time;
} Process;

typedef struct {
    Process* queue[MAX_PROCESSES];
    int front, rear;
} Queue;

int current_time = 0;
int queue_slices[MAX_QUEUES], queue_allotments[MAX_QUEUES];
Queue queues[MAX_QUEUES];
int aging_interval, num_queues, total_processes = 0;
Process processes[MAX_PROCESSES];

void init_queues() {
    for (int i = 0; i < MAX_QUEUES; i++) {
        queues[i].front = queues[i].rear = -1;
    }
}

void enqueue(Queue* q, Process* p) {
    if (q->rear == MAX_PROCESSES - 1) return;
    if (q->front == -1) q->front = 0;
    q->queue[++q->rear] = p;
}

Process* dequeue(Queue* q) {
    if (q->front == -1) return NULL;
    Process* p = q->queue[q->front];
    if (q->front == q->rear) q->front = q->rear = -1;
    else q->front++;
    return p;
}

void process_input(int argc, char* argv[]) {
    int index = 1;
    aging_interval = atoi(argv[index++]);
    num_queues = atoi(argv[index++]);

    for (int i = 0; i < num_queues; i++) {
        queue_slices[i] = atoi(argv[index++]);
        queue_allotments[i] = atoi(argv[index++]);
    }

    int i = 0;
    while (index < argc) {
        processes[i].id = 'A' + i;
        processes[i].arrival_time = atoi(argv[index++]);
        processes[i].burst_time = atoi(argv[index++]);
        processes[i].time_spent = 0;
        processes[i].queue_level = 0;
        processes[i].response_time = -1;
        total_processes++;
        i++;
    }
}

void schedule() {
    int completed_processes = 0;

    while (completed_processes < total_processes) {
        // Add arriving processes to Q1
        for (int i = 0; i < total_processes; i++) {
            if (processes[i].arrival_time == current_time) {
                enqueue(&queues[0], &processes[i]);
            }
        }

        // Aging: Move all processes back to Q1 every T time units
        if (current_time > 0 && current_time % aging_interval == 0) {
            for (int q = 1; q < num_queues; q++) {
                while (queues[q].front != -1) {
                    Process* p = dequeue(&queues[q]);
                    p->queue_level = 0;
                    enqueue(&queues[0], p);
                }
            }
        }

        // Find the highest-priority non-empty queue
        int q = 0;
        while (q < num_queues && queues[q].front == -1) q++;

        if (q < num_queues) {
            Process* p = dequeue(&queues[q]);

            // Calculate response time if this is the first run
            if (p->response_time == -1) {
                p->response_time = current_time - p->arrival_time;
            }

            // Determine run time (minimum of time slice and remaining burst time)
            int run_time = (p->burst_time - p->time_spent < queue_slices[q]) ? 
                           (p->burst_time - p->time_spent) : queue_slices[q];

            // Run the process for each unit of time in the time slice
            for (int i = 0; i < run_time; i++) {
                printf("%d %c\n", q + 1, p->id);
                p->time_spent++;
                current_time++;

                // Check if the process has completed
                if (p->time_spent == p->burst_time) {
                    p->turnaround_time = current_time - p->arrival_time;
                    printf("%c %d %d\n", p->id, p->response_time, p->turnaround_time);
                    completed_processes++;
                    break;
                }
            }

            // If the process hasn't completed, re-enqueue it
            if (p->time_spent < p->burst_time) {
                if (p->time_spent >= queue_allotments[q] && q + 1 < num_queues) {
                    p->queue_level++;
                    enqueue(&queues[q + 1], p);
                } else {
                    enqueue(&queues[q], p);
                }
            }
        } else {
            // No processes to run, increment time
            current_time++;
        }
    }

    // Calculate and print averages
    int total_response = 0, total_turnaround = 0;
    for (int i = 0; i < total_processes; i++) {
        total_response += processes[i].response_time;
        total_turnaround += processes[i].turnaround_time;
    }

    printf("%d %d %d\n", current_time, total_response / total_processes, total_turnaround / total_processes);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: ./proj1 T q S1 M1 ... Sq Mq P1a P1T ... Pna PnT\n");
        return 1;
    }

    init_queues();
    process_input(argc, argv);
    schedule();

    return 0;
}