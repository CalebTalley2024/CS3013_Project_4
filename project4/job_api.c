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

	if (workload -> head == NULL){ // if there are no jobs in the workload, simply put job in
		new_job->next = workload->head;
		workload->head = new_job;
		return 0;
	}
	if ((strcmp(p_type, "FIFO") == 0) || (strcmp(p_type, "RoundRobin") == 0))
	{

		new_job->next = workload->head;
		workload->head = new_job;
		return 0;
	}
	else if (strcmp(p_type, "SJF") == 0)  // by low to high LENGTH
	{ // order from smallest to largest LEN
		int new_job_len = new_job->len;
		Job *curr = workload->head;
		Job *prev = NULL;
		while (curr != NULL)
		{ // until we go through whole list
			// get length of new job and current job
			int curr_len = curr->len;
			printf("is %d < %d? \n",curr_len,new_job_len);

			if ( curr_len < new_job_len) // if job is larger than curr job, consider next job
			{ // check if new_job len is less than curr
				prev = curr;
				curr = curr -> next;
			}
			else // if the new job <= current job len, insert it in between prev and curr
			{
				new_job->next = curr;
				if (prev != NULL)
				{
						prev->next = new_job;
				}
				return 0;
			}
		}
		// edge case: tail of list/workload
		// prev is the end of the list after the while loop
		prev -> next = new_job;
		return 0;
	}
	else if (strcmp(p_type, "PRIO") == 0) // from low to high PRIORITY
	{ // order from smallest to largest LEN
			int new_job_prio = new_job->prio;
			Job *curr = workload->head;
			Job *prev = NULL;
			while (curr != NULL)
			{ // until we go through whole list
				// get length of new job and current job
				int curr_prio = curr->prio;
				printf("is %d < %d? \n",curr_prio,new_job_prio);
				if ( curr_prio < new_job_prio) // if job is larger than curr job, consider next job
				{ // check if new_job len is less than curr
					prev = curr;
					curr = curr -> next;
				}
				else // if the new job <= current job len, insert it in between prev and curr
				{
					new_job->next = curr;
					if (prev != NULL)
					{
							prev->next = new_job;
					}
					return 0;
				}
			}
			// edge case: tail of list/workload
			// prev is the end of the list after the while loop
			prev -> next = new_job;
			return 0;
		}
	else
	{
		perror("invalid priorty queue type");
		return -1;
	} 
}

// deletes job
// target_id: id of the job we want to delete
// minimum id value: 1
int delete_job(Workload *workload, int target_id)
{ // removes job based on id
	if (workload->head ==NULL)
	{ // checks if head is NULL
		perror("NULL pointer");
		return -1;
	};
	if (target_id == 1)
	{ // deletes head
		Job *temp = workload->head;
		workload->head = temp->next; // new head is what the next up job
		printf("removed id %d\n",temp -> id);
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
			if ((curr->id == target_id))
			{ // also updates curr_idx
				prev->next = curr->next;
				printf("removed id: %d\n",curr -> id);
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
	printf("End of Load \n\n");
}