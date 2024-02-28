#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "job_api.h"

Job *create_job(int id, int len, int prio)
{ // creates job node
	Job *new_job = (Job *)malloc(sizeof(Job));
	// set up pointers
	new_job->id = id;
	new_job->len = len;
	new_job->time_spent_waiting = 0; // this will start mattering when the p
	new_job->next = NULL;
	new_job->prio = prio;
	return new_job;
}
// adds job
// type: (FIFO, SJF, Priority,RR)
/*
*/
int add_job(Workload *workload, Job *new_job, const char p_type[])
{ // by default, adds job to the head of the list
	if ((strcmp(p_type, "FIFO") == 0) || (strcmp(p_type, "RoundRobin") == 0))
	{

		new_job->next = workload->head;
		workload->head = new_job;
		return 0;
	}
	else if (strcmp(p_type, "SJF") == 0)
	{ // order from smallest to largest LEN
		Job *curr = workload->head;
		Job *prev = NULL;
		while (curr != NULL)
		{ // until we go through whole list
			// get length of new job and current job
			int new_job_len = new_job->len;
			int curr_len = curr->len;
			if (new_job_len <= curr_len)
			{ // check if new_job len is less than curr
				if (prev == NULL)
				{ // Condition 1: make new job the new hear
					new_job->next = workload->head;
					workload->head = new_job;
					return 0;
				}
				else
				{ // Condition 2, insert job in between 2 jobs
					int prev_len = prev->len;
					// if new_job_len is in between prev_len and curr _len....
					//... insert new job in between prev_len and curr_len
					if (prev_len <= new_job_len)
					{
						prev->next = new_job;
						new_job->next = curr;
						return 0;
					}
				}
			}
			// update prev
			prev = curr;
			curr = curr->next;
		}
		printf("idx is out of bounds");
		return 1;
	}
	else if (strcmp(p_type, "PRIO") == 0)
	{ // order from smallest to largest Priority
		Job *curr = workload->head;
		Job *prev = NULL;
		while (curr != NULL)
		{ // until we go through whole list
			// get length of new job and current job
			int new_job_prio = new_job->prio;
			int curr_prio = curr->prio;
			if (new_job_prio <= curr_prio)
			{ // check if new_job len is less than curr
				if (prev == NULL)
				{ // Condition 1: make new job the new hear
					new_job->next = workload->head;
					workload->head = new_job;
					return 0;
				}
				else
				{ // Condition 2, insert job in between 2 jobs
					int prev_prio = prev->prio;
					// if new_job_len is in between prev_len and curr _len....
					//... insert new job in between prev_len and curr_len
					if (prev_prio <= new_job_prio)
					{
						prev->next = new_job;
						new_job->next = curr;
						return 0;
					}
				}
			}
			// update prev
			prev = curr;
			curr = curr->next;
		}
		printf("idx is out of bounds");
		return 1;
	}
	else
	{
		printf("Non-Valid Priority Queue");
		return 2;
	}
}

// deletes job
// target_id: id of the job we want to delete
int delete_job(Workload *workload, int target_id)
{ // removes job based on id
	if (workload->head ==NULL)
	{ // checks if head is NULL
		perror("NULL pointer");
		return -1;
	};
	if (target_id == 0)
	{ // deletes head
		Job *temp = workload->head;
		workload->head = temp->next; // new head is what the next up job
		printf("removed id %d",temp -> id);
		free(temp);					 // free meory
		return 0;
	}
	else
	{
		// create current and previous pointers
		Job *curr = workload->head;
		Job *prev = NULL; // note that prev will never be NULL when trying prev -> next, hence ther is no error checking
		while (curr != NULL)
		{ // find index
			prev = curr;
			curr = curr->next;
			if (curr->id == target_id)
			{ // also updates curr_idx
				prev->next = curr->next;
				printf("removed id %d",curr -> id);
				free(curr); // delete memory associate with the job
				return 0;
			}
		}
		printf("idx is out of bounds");
		return 1;
	}
}

// ordered add
int reverse_workload(Workload *workload)
{
	Job *head = workload->head;
	Job *prev = NULL, *curr = head, *next_node;

	while (curr != NULL)
	{
		next_node = curr->next; // shift to right
		curr->next = prev;		// point to left
		prev = curr;			// after init to head, shift to right
		curr = next_node;		// shift to right
	}
	// at this point prev is the first job, and curr is null
	workload->head = prev;
	return 0;
}

// prints the list, line by line
void print_workload(Workload *workload)
{
	Job *curr = workload->head;
	while (curr != NULL)
	{
		printf("Job ID: %d, Length: %d, Priority: %d\n", curr->id, curr->len, curr -> prio);
		curr = curr->next;
	}
	printf("End of Load \n");
}