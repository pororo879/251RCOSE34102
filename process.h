#ifndef PROCESS_H   // 중복 정의 방지
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ID_LEN 5    // 최대 프로세스 ID 길이
typedef int Quantum;    // Round Robin용 time quantum 타입

// 프로세스 구조체
typedef struct {
    char    id[ID_LEN];        // 프로세스 ID
    int     arrival_time;      // 도착 시간
    int     burst_time;          // 총 실행 시간
    int     remaining_time;    // 남은 실행 시간
    int     priority;          // 우선순위
    int     waiting_time;      // 대기 시간
    int     response_time;     // 시작 시간
    int     return_time;       // 완료 시간
    int     turnaround_time;   // Turnaround 시간
    bool    completed;         // 완료 여부

    // io 변수들 (io 구현 안함)
    //int     io_request_time;
    //int     io_burst_time;
} Process;

/*
 Process_init
    프로세스 배열을 돌면서
    waiting/response/turnaround/return_time을 0으로,
    remaining_time을 burst_time으로,
    completed를 false로 초기화함.
 */
void Process_init(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].response_time = 0;
        p[i].turnaround_time = 0;
        p[i].return_time = 0;
        p[i].remaining_time = p[i].burst_time;
        p[i].completed = false;
    }
}

#endif 