#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"
#include "fcfs.h"
#include "sjf.h"
#include "npps.h"
#include "srtf.h"
#include "pps.h"
#include "rr.h"
#include "hrrn.h"
#include "config.h"


int main() {

    srand((unsigned int)time(NULL)); // 랜덤 시드 초기화

    int process_count = 4 + rand() % 3; // 4~6, 나머지에 4쁠
    Quantum quantum; // process.h 참고

    printf("Number of Processes: %d\n", process_count);
    Process* process = (Process*)malloc(sizeof(Process) * process_count); // 프로세스 메모리 동적할당
    if (process == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    quantum = 4 + rand() % 3;  // 4~6
    printf("Time Quantum for Round Robin: %d\n", quantum);

    printf("\n[Auto-generated Process Data]\n");
    for (int i = 0; i < process_count; i++) {
        snprintf(process[i].id, ID_LEN, "p%d", i + 1);
        process[i].arrival_time = i;                // 0 ~ process_count
        process[i].burst_time = 1 + rand() % 20;    // 1 ~ 20
        process[i].priority = 1 + rand() % process_count;   // 1 ~ process_count

        printf("Process %s: arrival=%d, burst=%d, priority=%d\n",
            process[i].id, process[i].arrival_time, process[i].burst_time, process[i].priority);
    }

    // io 데이터 (io 구현 안함)
    //process[0].io_request_time = process[0].burst_time / 2;
    //process[0].io_burst_time = 2 + rand() % 4; // 2~5
    //printf("I/O request: %d, I/O burst: %d\n", process[0].io_request_time, process[0].io_burst_time);


    Process_init(process, process_count);

    printf("\n");
    FCFS_Scheduling(process, process_count); printf("\n");
    SJF_Scheduling(process, process_count); printf("\n");
    NPPS_Scheduling(process, process_count); printf("\n");
    SRTF_Scheduling(process, process_count); printf("\n");
    PPS_Scheduling(process, process_count); printf("\n");
    RR_Scheduling(process, process_count, quantum); printf("\n");
    HRRN_Scheduling(process, process_count); printf("\n");

    free(process); // 메모리 해제
    return 0;
}
