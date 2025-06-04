#ifndef RR_H
#define RR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include "config.h"


// 간트 차트 출력
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

    // 도착 시간 순으로 정렬 (like FCFS)
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
    init_queue(&q);  // 큐 초기화
    bool enqued[10];  // 중복 방지
    memset(enqued, 0, sizeof(enqued));  // enqued 배열 0으로 메모리 초기화(false)

    char executed_ids[100][ID_LEN];
    int executed_time[101];  // 종료 시간 포함
    int gantt_index = 0;

    while (completed_count < n) {
        for (int i = 0; i < n; i++) {  
            // 도착 프로세스 큐에 추가
            if (p[i].arrival_time <= current_time && !enqued[i]) {  // 도착했고 아직 엔큐 안된 프로세스들만 고려
                enqueue(&q, &p[i]);
                enqued[i] = true;  // 엔큐했으니 true로 바꿈
            }
        }

        if (!is_empty(&q)) {  // 큐 안 비었으면 디큐
            Process* curr = dequeue(&q);  // 디큐 프로세스 포인터 curr

            executed_time[gantt_index] = current_time;
            strcpy(executed_ids[gantt_index], curr->id);  // 포인터가 id 필드 접근해서 executed_ids 배열에 저장
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


            // 새로 도착한 프로세스 enqueue하기
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time <= current_time && !enqued[i]) {
                    enqueue(&q, &p[i]);
                    enqued[i] = true;
                }
            }

            // 실행 시간 남았으면 다시 enqueue하기
            if (curr->remaining_time > 0) {
                enqueue(&q, curr);
            }
            else {  // 실행 완료인 경우
                curr->return_time = current_time;
                curr->turnaround_time = curr->return_time - curr->arrival_time;
                curr->waiting_time = curr->turnaround_time - curr->burst_time;
                curr->completed = true;
                completed_count++;
            }
        }
        else {  // 큐 empty인 경우, 즉 idle
            current_time++;
        }
    }

    executed_time[gantt_index] = current_time; // 마지막 시간 기록

    RR_gantt_chart(executed_ids, executed_time, gantt_index);
    RR_evaluate(p, n);
}

#endif
