#ifndef PROCESS_H   // �ߺ� ���� ����
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ID_LEN 5    // �ִ� ���μ��� ID ����
typedef int Quantum;    // Round Robin�� time quantum Ÿ��

// ���μ��� ����ü
typedef struct {
    char    id[ID_LEN];        // ���μ��� ID
    int     arrival_time;      // ���� �ð�
    int     burst_time;          // �� ���� �ð�
    int     remaining_time;    // ���� ���� �ð�
    int     priority;          // �켱����
    int     waiting_time;      // ��� �ð�
    int     response_time;     // ���� �ð�
    int     return_time;       // �Ϸ� �ð�
    int     turnaround_time;   // Turnaround �ð�
    bool    completed;         // �Ϸ� ����

    // io ������ (io ���� ����)
    //int     io_request_time;
    //int     io_burst_time;
} Process;

/*
 Process_init
    ���μ��� �迭�� ���鼭
    waiting/response/turnaround/return_time�� 0����,
    remaining_time�� burst_time����,
    completed�� false�� �ʱ�ȭ��.
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