/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))
#define FACTOR_A 0.5

/*global metablhth gia thn apothikeysh toy xronoy amesos meta to context_switch */
static long long current_time;
static struct task_struct *nxt;

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;
extern struct task_struct *idle;

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
 /* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */
void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	seedTask->next = seedTask->prev = seedTask;
	newrq->head = seedTask;
	newrq->nr_running++;
	
	/*arxikopoihsh toy current_time */
	current_time = 0;
	nxt = NULL;
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
{
	//an kapoia einai mesa sthn lista wq prepei na thn kanoyme free nomizo
	return;
}

/*na ginei opvw htan arxika*/
void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr)
		printf("%p", curr);
	printf("\nINIT %p\n", rq->head);
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %p", curr);
		printf("burst_time: %llu expected_burst: %llu goodness: %llu\n",curr->burst, curr->exp_burst, curr->goodness);
	};
	printf("\n");
}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
	struct task_struct *curr;
	struct task_struct *min_curr;
	long long min_burst,max_wait;
	long long min_goodness=0;
	long long current_wait;
	int i = 0;
	
	printf("In schedule\n");
	
	
	current->waiting = sched_clock();
	current->need_reschedule = 0; /* Always make sure to reset that, in case *
								   * we entered the scheduler because current*
								   * had requested so by setting this flag   */
	
	current_wait = sched_clock();
	curr = rq->head->next;
	min_burst = curr->exp_burst + 1;
	max_wait = 0;
	
	while(curr!=rq->head){
		if(min_burst > ( curr->exp_burst + 1 )){
			min_burst = curr->exp_burst + 1;
		}
		if(max_wait < (current_wait - curr->waiting + 1)){
			max_wait = current_wait - curr->waiting + 1;
		}
		curr = curr->next;
	}
	
	curr = rq->head->next;
	while(curr!=rq->head){
		curr->goodness = ((1 + curr->exp_burst) * max_wait) / (min_burst * (1 + (current_wait - curr->waiting)));
		curr = curr->next;
	}
	
	
	print_rq();
	
	if(rq->nr_running == 1){
		context_switch(rq->head);
		nxt = rq->head->next;
		curr = nxt;
	}
	else{
		if(nxt == rq->head){
			nxt = nxt->next;
		}
		
		curr = nxt;
		min_goodness = curr->goodness;
		
		for(i=0;i<rq->nr_running;i++){
			
			if(curr == rq->head){
				curr = curr->next;
				continue;
			}
			
			if(min_goodness >= curr->goodness){
				min_goodness = curr->goodness;
				min_curr = curr;
			}
			
			curr = curr->next;
		}
		nxt = min_curr;
		printf("RUNNING: %p\n", min_curr);
		context_switch(min_curr);
		current_time = sched_clock();
	}
}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 100;
	p->exp_burst = 0;
	p->burst = 0;
	p->goodness = 0;
	p->waiting = sched_clock();
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{
	/*orismos toy burst_time*/
	p->burst = sched_clock() - current_time;
	/*ypologismos tou exp_burst*/
	p->exp_burst = (p->burst + (FACTOR_A*(p->exp_burst)))/(1+FACTOR_A);
	
	schedule();
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	rq->nr_running++;
	
	p->waiting = sched_clock();
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	if(nxt == p){
		nxt = p->next;
	}
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
	* next is checked in cpu.c      */
	rq->nr_running--;
}