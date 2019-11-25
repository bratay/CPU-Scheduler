/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"
#include "../libpriqueue/libpriqueue.h"

scheduler_t scheduler;

/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job queue elements. 
*/
typedef struct _job_t
{
  int runTime, firstRun, priority, timeLeft, jobID, arrival;
} job_t;


// Comparison for schemes


int PSJF_comp(const job_t *in_queue, const job_t *new_job)
{
  return in_queue->timeLeft - new_job->timeLeft;
}

int FCFS_comp(__attribute__((unused)) const job_t *in_queue, __attribute__((unused)) const job_t *new_job){ return -1; }

int priComp(const job_t *in_queue, const job_t *new_job)
{
  if (in_queue->firstRun != -1)
    return -1;
  else
    return in_queue->priority - new_job->priority;
}

int SJF_comp(const job_t *in_queue, const job_t *new_job)
{
  if (in_queue->firstRun != -1)
    return -1;
  else
    return in_queue->runTime - new_job->runTime;
}

int RR_comp(__attribute__((unused)) const job_t *in_queue, __attribute__((unused)) const job_t *new_job){ return -1; }

int PpriComp(const job_t *in_queue, const job_t *new_job){ return in_queue->priority - new_job->priority; }

/**
   Helper function to determine the comparison function to use based on a user picked scheme.
   @param scheme - scheme chosen by user
   @return a function pointer for comparing two jobs
*/
comp_t get_comparer(scheme_t scheme)
{
  switch (scheme)
  {
  case PRI:
    return (comp_t)priComp;
  case PSJF:
    return (comp_t)PSJF_comp;
  case SJF:
    return (comp_t)SJF_comp;
  case PPRI:
    return (comp_t)PpriComp;
  case FCFS:
    return (comp_t)FCFS_comp;
  case RR:
    return (comp_t)RR_comp;
  default:
    return NULL;
  }
}

/**
  Initalizes the scheduler.
 
  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
  scheduler.jobsRun = 0;
  scheduler.waitTime = (int *)malloc(MIN_SIZE_ARRS * sizeof(int));
  scheduler.reponses = (int *)malloc(MIN_SIZE_ARRS * sizeof(int));
  scheduler.turnAroundTimes = (int *)malloc(MIN_SIZE_ARRS * sizeof(int));
  scheduler.finishedJobs = 0;
  scheduler.arrSize = MIN_SIZE_ARRS;

  scheduler.scheme = scheme;
  scheduler.availableCores = cores;
  scheduler.priComp = get_comparer(scheme);
  priqueue_init(&scheduler.jobs, scheduler.priComp);
}

void checkResponses(int time)
{
  job_t *job = priqueue_peek(&scheduler.jobs);
  
  if (job == NULL)
    return;
  else if (job->firstRun == -1)
  {
    job_t *preFirstJob = priqueue_at(&scheduler.jobs, 1);

    if (preFirstJob != NULL && preFirstJob->firstRun == time)
      preFirstJob->firstRun = -1;
    else
      scheduler.jobsRun++;

    if (scheduler.jobsRun == scheduler.arrSize)
    {
      scheduler.turnAroundTimes = realloc(scheduler.turnAroundTimes, scheduler.arrSize * sizeof(int));
      scheduler.waitTime = realloc(scheduler.waitTime, scheduler.arrSize * sizeof(int));
      scheduler.reponses = realloc(scheduler.reponses, scheduler.arrSize * sizeof(int));
      scheduler.arrSize *= SCALE_SIZE;
    }

    job->firstRun = time;

    scheduler.reponses[scheduler.jobsRun - 1] = time - job->arrival;
  }
}

/**
  Called when a new job arrives.
 
  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made. 
 
 */
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
  int currentRunning = -1;

  job_t *currentJob = priqueue_peek(&scheduler.jobs);

  if (currentJob != NULL)
  {
    currentRunning = currentJob->jobID;
    currentJob->timeLeft = currentJob->timeLeft - (time - currentJob->firstRun);
  }

  job_t *new_job = (job_t *)malloc(1 * sizeof(job_t));
  new_job->priority = priority;
  new_job->jobID = job_number;
  new_job->arrival = time;
  new_job->firstRun = -1;
  new_job->runTime = running_time;
  new_job->timeLeft = running_time;

  priqueue_offer(&scheduler.jobs, new_job);
  checkResponses(time);

  // tentative, update this if multiple core option is done
  if (((job_t *)priqueue_peek(&scheduler.jobs))->jobID != currentRunning)
    return 0;
  else
    return -1;
}

/**
  Called when a job has completed execution.
 
  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.
 
  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
	return -1;
}


/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.
 
  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator. 
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time)
{
	return -1;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
	return 0.0;
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
	return 0.0;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
	return 0.0;
}


/**
  Free any memory associated with your scheduler.
 
  Assumption:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{

}


/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)  
  
  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{

}
