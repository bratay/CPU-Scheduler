/** @file queuetest.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "libscheduler/libscheduler.h"

int main()
{
	scheduler_t sch;
    float turnaroundTime, waitTime, responseTime;
    
    //FCFS scheme
    scheduler_start_up(4, FCFS);

    //turn around time
    printf("Expected turnaround time: \n");
    turnaroundTime = scheduler_average_turnaround_time();
    printf("Actual turnaround Time %f \n", turnaroundTime);
    
    //wait time
    printf("Expected wait time:\n");
    waitTime = scheduler_average_waiting_time();
    printf("Actual wait Time %f \n", waitTime);

    //response time
    printf("Expected response time: \n");
    responseTime = scheduler_average_response_time();
    printf("Actual response Time %f \n", responseTime);

    scheduler_show_queue();
    scheduler_clean_up();
    
    //RR scheme
    scheduler_start_up(4, RR);

    //turn around time
    printf("Expected turnaround time: \n");
    turnaroundTime = scheduler_average_turnaround_time();
    printf("Actual turnaround Time %f \n", turnaroundTime);
    
    //wait time
    printf("Expected wait time:\n");
    waitTime = scheduler_average_waiting_time();
    printf("Actual wait Time %f \n", waitTime);

    //response time
    printf("Expected response time: \n");
    responseTime = scheduler_average_response_time();
    printf("Actual response Time %f \n", responseTime);

    scheduler_show_queue();
    scheduler_clean_up();

	return 0;
}