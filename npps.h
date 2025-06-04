#ifndef NPPS_H
#define NPPS_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "process.h"

// ��Ʈ ��Ʈ ���
void NPPS_gantt_chart(Process* p, int n) {
    printf("\nGantt Chart (NPPS):\n");
    printf("|");

    for (int i = 0; i < n; i++) {
        int width = p[i].burst_time * 2;
        printf("%s", p[i].id);
        int id_len = strlen(p[i].id);
        for (int j = 0; j < width - id_len; j++)
            printf(" ");
        printf("|");
    }
    printf("\n");

    // �ð� ���
    int current_time = 0;
    printf("0");
    for (int i = 0; i < n; i++) {
        int width = p[i].burst_time * 2;
        for (int j = 0; j < width; j++)
            printf(" ");
        current_time += p[i].burst_time;
        if (current_time >= 10) printf("\b");
        printf("%d", current_time);
    }
    printf("\n");
}

// Evaluation
void NPPS_evaluate(Process* p, int n) {
    double total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
    }

    printf("\n[NPPS Evaluation]\n");
    printf("Average Waiting Time: %.2lf\n", total_wait / n);
    printf("Average Turnaround Time: %.2lf\n", total_turn / n);
}



// Non-Preemptive Priority Scheduling
void NPPS_Scheduling(Process* p, int n) {
    Process_init(p, n);

    int completed_count = 0;
    int current_time = 0;

    while (completed_count < n) {
        int idx = -1;
        int min_priority = INT_MAX;  // min_priority �ʱⰪ (���ڰ� �������� higher priority)

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !p[i].completed) {  // �����߰� �Ϸ� �ƴ� ���μ����鸸 ���, �� �� min_priority ���μ��� ��
                if (p[i].priority < min_priority) {
                    min_priority = p[i].priority;
                    idx = i;
                }
                else if (p[i].priority == min_priority && p[i].arrival_time < p[idx].arrival_time) {  // priority ������ ���� ������ ���μ��� ��
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            p[idx].waiting_time = current_time - p[idx].arrival_time;
            p[idx].return_time = current_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].return_time - p[idx].arrival_time;
            current_time += p[idx].burst_time;
            p[idx].completed = true;
            completed_count++; 
        }
        else {  // idle
            current_time++;
        }
    }

    // ���� ���� ���� (�Ϸ��)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].return_time > p[j].return_time) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    NPPS_gantt_chart(p, n);
    NPPS_evaluate(p, n);
}

#endif
