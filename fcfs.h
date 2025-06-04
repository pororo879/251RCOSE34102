#ifndef FCFS_H
#define FCFS_H

#include <stdio.h>
#include <string.h>
#include "process.h"

// 간트 차트 출력
void FCFS_gantt_chart(Process* p, int n) {
    printf("\nGantt Chart (FCFS):\n");
    printf("|");

    for (int i = 0; i < n; i++) {
        int width = p[i].burst_time * 2;
        printf("%s", p[i].id);

        int id_len = strlen(p[i].id);
        for (int j = 0; j < width - id_len; j++)  // pid 길이 빼고 공백 출력  
            printf(" ");
        printf("|");
    }
    printf("\n");

    // 시간 출력
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
void FCFS_evaluate(Process* p, int n) {
    double total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        total_wait += p[i].waiting_time;    // 스케줄링 함수에서 값 읽어옴
        total_turn += p[i].turnaround_time;
    }

    printf("\n[FCFS Evaluation]\n");
    printf("Average Waiting Time: %.2lf\n", total_wait / n);
    printf("Average Turnaround Time: %.2lf\n", total_turn / n);
}



// First Come First Served
void FCFS_Scheduling(Process* p, int n) {
    Process_init(p, n);

    // 도착 시간대로 정렬
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival_time > p[j].arrival_time) {
                Process temp = p[i];  // swap
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;  // idle 이면 arrival_time으로 점프  

        p[i].waiting_time = current_time - p[i].arrival_time;
        p[i].return_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].return_time - p[i].arrival_time;
        p[i].completed = true;

        current_time += p[i].burst_time;
    }

    FCFS_gantt_chart(p, n);
    FCFS_evaluate(p, n);
}

#endif