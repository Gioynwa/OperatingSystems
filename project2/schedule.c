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

/*currently running with Exp Burst - put this line in comments to run with Goodness*/
#define exp 0
/*--------------------------------------------------------------------------------*/

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/*------------------*/
double min_exp_burst;
double max_wait_rq;
double min_goodness;
/*------------------*/
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
	
	/*---------------*/
	current = seedTask;
	rq = newrq;
	/*---------------*/
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
{
	return;
}


void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr)
		printf("%p", curr);
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %p", curr);
	};
	printf("\n");
}

void values_update(struct task_struct *curr) {

	current->proc_leave_time = (double)sched_clock();
	printf("curr!=current, current->proc_leave_time %lf\n", current->proc_leave_time);

	current->burst = current->proc_leave_time - current->proc_take_time;
	printf("curr!=current, current->proc_take_time %lf\n", current->proc_take_time);

	current->exp_burst = (current->burst + 0.5*current->exp_burst)*0.67;
	printf("curr!=current, current->exp_burst %lf\n", current->exp_burst);

	current->wait_rq = current->proc_leave_time - current->enter_rq;
	printf("curr!=current, current->wait_rq %lf\n", current->wait_rq);

	current->goodness = ((double)(current->exp_burst + 1)/(double)(1 + min_exp_burst)) \
		             * ((double)(1 + max_wait_rq)/(double)(1 + current->wait_rq));
	printf("curr!=current->goodness %lf\n", current->goodness);
}


/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
	struct task_struct *curr, *temp_exp, *temp_good;
	
	printf("In schedule\n");
	print_rq();
	
	current->need_reschedule = 0; /* Always make sure to reset that, in case *
				       * we entered the scheduler because current*
				       * had requested so by setting this flag   */
	
	if (rq->nr_running == 1) {
		
		values_update(current);
		
		context_switch(rq->head);
		
		current->proc_take_time = (double)sched_clock();
		
	}
	else {	
			
		// nxt = nxt->next;
		// if (nxt == rq->head)          /* Do this to always skip init at the head */
		//	nxt = nxt->next;	/* of the queue, whenever there are other  */
								

		/*------------ calc min_exp_burst -----------*/
		curr = rq->head;
		min_exp_burst = curr->next->exp_burst;
		
		while(curr->next != rq->head) {
			curr = curr->next;
			if(curr->exp_burst < min_exp_burst) {	
				min_exp_burst = curr->exp_burst;
			}
		};
		curr = rq->head;
		while(curr->next != rq->head) {
			curr = curr->next;
			if(min_exp_burst == curr->exp_burst) {
				break;
			}
		};
		
		temp_exp = curr;
		printf("curr = %p, exp_burst = %lf\n", temp_exp, temp_exp->exp_burst);

		/*------------------------- calc wait_rq and max_wait_rq ----------------------------*/
		curr = rq->head;
		while(curr->next != rq->head){
			curr = curr->next;
			curr->wait_rq = (double)sched_clock() - curr->enter_rq;		
		}		
		
		curr = rq->head;
		max_wait_rq = curr->next->wait_rq;	
		while(curr->next != rq->head) {
			curr = curr->next;
			if(curr->wait_rq > max_wait_rq) {	
				max_wait_rq = curr->wait_rq;
			}
		};
		curr = rq->head;
		while(curr->next != rq->head) {
			curr = curr->next;
			if(max_wait_rq == curr->wait_rq) {
				break;
			}
		};
		

		/*------------------------ calculate goodness values of all nodes--------------------------*/
		curr = rq->head;
		while(curr->next != rq->head){

			curr = curr->next;
			curr->goodness = ((double)(curr->exp_burst + 1)/(double)(1 + min_exp_burst)) \
					 * ((double)(1 + max_wait_rq)/(double)(1 + curr->wait_rq));	

			printf("calc all:\n curr->goodness %lf\n", curr->goodness);
		}
		/*----------------------------------------------------------------------------------------*/
		

		/*--------------------------------calc min goodness--------------------------------------*/
		curr = rq->head;
		min_goodness = curr->next->goodness;
		while(curr->next != rq->head) {
			curr = curr->next;
			if(curr->goodness < min_goodness) {	
				min_goodness = curr->goodness;
			}
		};
		curr = rq->head;
		while(curr->next != rq->head) {
			curr = curr->next;
			if(min_goodness == curr->goodness) {
				break;
			}
		};

		temp_good = curr;
		/*---------------------------------------------------------------------------------------*/

		printf("curr = %p,min_goodness = %lf,curr->goodness=%lf curr->wait_rq=%lf\n", \
					temp_good, min_goodness,curr->goodness,curr->wait_rq);
		

		/* ----------Making sure to update the values of a node before the context---------------*
		  *------------switch if the current task is going to be replaced (by curr)-------------- */
		#ifdef exp		
		if(temp_exp != current) {

			values_update(temp_exp);
		}
		#endif
		
		#ifndef exp
		if(temp_good != current) {

			values_update(temp_good);
		}
		/*-----------------------------------------------------------------------------------------*/
		#endif		

		#ifdef exp
			printf("Exp burst running!\n");
			context_switch(temp_exp);
		#endif
		
		#ifndef exp
			printf("Goodness running!\n");
			context_switch(temp_good);
		#endif
	}
}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 100;

	/*--initializations--*/
	p->burst = 0;
	p->exp_burst = 0;
	p->proc_take_time = 0;
	p->proc_leave_time = 0;
	p->enter_rq = 0;
	/*--------------------*/
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{
	schedule();
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	printf("in wakeup task\n");
	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	rq->nr_running++;
	
	printf("out wakeup task\n");
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	printf("in activate task\n");

	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	rq->nr_running++;

	p->enter_rq = (double)sched_clock();
	
	printf("out activate task\n");
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	printf("in deactivate task\n");

	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
				   * next is checked in cpu.c      */

	rq->nr_running--;

	/*-----------Update values of current proccess before its deactivation--------------*/
	values_update(current);

	/*we refresh the proc_take_time here because is the only time 
	*we are sure that another process will take the processor afterwards*/
	current->proc_take_time = (double)sched_clock();
	/*---------------------------------------------------------------------------------*/
	printf("out deactivate task\n");
}
