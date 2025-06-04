#ifndef RR_H
#define RR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include "config.h"


// ��Ʈ ��Ʈ ���
void RR_gantt_chart(char executed_ids[][ID_LEN], int executed_time[], int count) {
    printf("\nGantt Chart (RR):\n");
    printf("|");

    for (int i = 0; i < count; i++) {
        int width = (executed_time[i + 1] - executed_time[i]) * 2;
        printf("%s", executed_ids[i]);
        int id_len = strlen(executed_ids[i]);
        for (int j = 0; j < width - id_len; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n");

    // �ð� ���
    printf("%d", executed_time[0]);  // 0 ���
    for (int i = 1; i <= count; i++) {
        int width = (executed_time[i] - executed_time[i - 1]) * 2;
        for (int j = 0; j < width; j++)
            printf(" ");
        if (executed_time[i] >= 10) printf("\b");  // �� �ڸ��� ����
        printf("%d", executed_time[i]);
    }
    printf("\n");
}

// Evaluation
void RR_evaluate(Process* p, int n) {
    double total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
    }

    printf("\n[RR Evaluation]\n");
    printf("Average Waiting Time: %.2lf\n", total_wait / n);
    printf("Average Turnaround Time: %.2lf\n", total_turn / n);
}


// Round Robin 
void RR_Scheduling(Process* p, int n, Quantum quantum) {
    Process_init(p, n);

    // ���� �ð� ������ ���� (like FCFS)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival_time > p[j].arrival_time) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int completed_count = 0, current_time = 0;
    Queue q;
    init_queue(&q);  // ť �ʱ�ȭ
    bool enqued[10];  // �ߺ� ����
    memset(enqued, 0, sizeof(enqued));  // enqued �迭 0���� �޸� �ʱ�ȭ(false)

    char executed_ids[100][ID_LEN];
    int executed_time[101];  // ���� �ð� ����
    int gantt_index = 0;

    while (completed_count < n) {
        for (int i = 0; i < n; i++) {  
            // ���� ���μ��� ť�� �߰�
            if (p[i].arrival_time <= current_time && !enqued[i]) {  // �����߰� ���� ��ť �ȵ� ���μ����鸸 ���
                enqueue(&q, &p[i]);
                enqued[i] = true;  // ��ť������ true�� �ٲ�
            }
        }

        if (!is_empty(&q)) {  // ť �� ������� ��ť
            Process* curr = dequeue(&q);  // ��ť ���μ��� ������ curr

            executed_time[gantt_index] = current_time;
            strcpy(executed_ids[gantt_index], curr->id);  // �����Ͱ� id �ʵ� �����ؼ� executed_ids �迭�� ����
            gantt_index++;

            int run_time;
            if (curr->remaining_time > quantum) {
                run_time = quantum;
            }
            else {
                run_time = curr->remaining_time;
            }

            curr->remaining_time -= run_time;
            current_time += run_time;


            // ���� ������ ���μ��� enqueue�ϱ�
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time <= current_time && !enqued[i]) {
                    enqueue(&q, &p[i]);
                    enqued[i] = true;
                }
            }

            // ���� �ð� �������� �ٽ� enqueue�ϱ�
            if (curr->remaining_time > 0) {
                enqueue(&q, curr);
            }
            else {  // ���� �Ϸ��� ���
                curr->return_time = current_time;
                curr->turnaround_time = curr->return_time - curr->arrival_time;
                curr->waiting_time = curr->turnaround_time - curr->burst_time;
                curr->completed = true;
                completed_count++;
            }
        }
        else {  // ť empty�� ���, �� idle
            current_time++;
        }
    }

    executed_time[gantt_index] = current_time; // ������ �ð� ���

    RR_gantt_chart(executed_ids, executed_time, gantt_index);
    RR_evaluate(p, n);
}

#endif
