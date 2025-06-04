#ifndef SRTF_H
#define SRTF_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "process.h"

// 간트 차트 출력
void SRTF_gantt_chart(char executed_ids[][ID_LEN], int executed_time[], int count) {  // count = gantt_index
    printf("\nGantt Chart (SRTF):\n");
    printf("|");

    for (int i = 0; i < count; i++) {
        int width = (executed_time[i + 1] - executed_time[i]) * 2;
        printf("%s", executed_ids[i]);
        int id_len = strlen(executed_ids[i]);
        for (int j = 0; j < width - id_len; j++)
            printf(" ");
        printf("|");
    }
    printf("\n");

    // 시간 출력
    printf("%d", executed_time[0]);  // 0 출력  
    for (int i = 1; i <= count; i++) {
        int width = (executed_time[i] - executed_time[i - 1]) * 2;
        for (int j = 0; j < width; j++)
            printf(" ");
        if (executed_time[i] >= 10) printf("\b");  // 두 자리수 보정
        printf("%d", executed_time[i]);
    }
    printf("\n");
}


// Evaluation
void SRTF_evaluate(Process* p, int n) {
    double total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
    }

    printf("\n[SRTF Evaluation]\n");
    printf("Average Waiting Time: %.2lf\n", total_wait / n);
    printf("Average Turnaround Time: %.2lf\n", total_turn / n);
}

// Shortest Remaining Time First - Preemptive
void SRTF_Scheduling(Process* p, int n) {
    Process_init(p, n);

    int current_time = 0, completed_count = 0;
    int prev_idx = -1;  // 초기값
    char executed_ids[100][ID_LEN];
    int executed_time[101];  // 종료 시간 포함
    int gantt_index = 0;

    while (completed_count < n) {
        int idx = -1;
        int min_remaining = INT_MAX; 

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !p[i].completed && p[i].remaining_time > 0) {  // 도착했고 완료 아니고 remaining_time 남은 프로세스들만 고려, 그 중 min_remaining 프로세스 택
                if (p[i].remaining_time < min_remaining ||
                    (p[i].remaining_time == min_remaining && (idx == -1 || p[i].arrival_time < p[idx].arrival_time))) {  // remaining_time 같으면 더 먼저 도착한 프로세스 택
                    min_remaining = p[i].remaining_time;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            if (idx != prev_idx) {  // p1 -> p2 처럼 프로세스 바뀐 경우에만 실행
                strncpy(executed_ids[gantt_index], p[idx].id, ID_LEN - 1);  // pid를 배열에 저장
                executed_ids[gantt_index][ID_LEN - 1] = '\0';  // 문자열 배열이라 끝에 \0으로 안전종료
                executed_time[gantt_index] = current_time;  // current_time을 배열에 저장
                gantt_index++;
                prev_idx = idx;
            }

            p[idx].remaining_time--;
            current_time++;

            if (p[idx].remaining_time == 0) {  // 프로세스 완료
                p[idx].completed = true;
                p[idx].return_time = current_time;
                p[idx].turnaround_time = p[idx].return_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                completed_count++;
            }
        }
        else {  // idx = -1인 경우, 즉 idle
            if (prev_idx != -2) {  // 이전 프로세스도 idle 이었으면 패스
                strncpy(executed_ids[gantt_index], "IDLE", ID_LEN - 1);
                executed_ids[gantt_index][ID_LEN - 1] = '\0';
                executed_time[gantt_index] = current_time;
                gantt_index++;
                prev_idx = -2;
            }
            current_time++;
        }
    }

    executed_time[gantt_index] = current_time; // 마지막 시간 기록

    SRTF_gantt_chart(executed_ids, executed_time, gantt_index);
    SRTF_evaluate(p, n);
}

#endif
