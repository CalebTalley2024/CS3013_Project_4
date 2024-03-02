#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "job_api.h"

Job *create_job(int id, int len, int prio)
{ // creates job node
	Job *new_job = (Job *)malloc(sizeof(Job));
	// set up pointers
	new_job->id = id;
	new_job->len = len;
	// new_job->time_spent_waiting = 0; // this will start mattering when the p
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

	if (workload->head == NULL)
	{ // if there are no jobs in the workload, simply put job in
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
	else if (strcmp(p_type, "SJF") == 0) // by low to high LENGTH
	{									 // order from smallest to largest LEN
		int new_job_len = new_job->len;
		Job *curr = workload->head;
		Job *prev = NULL;
		while (curr != NULL)
		{ // until we go through whole list
			// get length of new job and current job
			int curr_len = curr->len;
			// printf("is %d < %d? \n",curr_len,new_job_len);

			if (curr_len <= new_job_len) // if job is larger than curr job, consider next job
			{							 // check if new_job len is less than curr
				prev = curr;
				curr = curr->next;
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
		prev->next = new_job;
		return 0;
	}
	else if (strcmp(p_type, "PRIO") == 0) // from low to high PRIORITY
	{									  // order from smallest to largest LEN
		int new_job_prio = new_job->prio;
		Job *curr = workload->head;
		Job *prev = NULL;
		while (curr != NULL)
		{ // until we go through whole list
			// get length of new job and current job
			int curr_prio = curr->prio;
			// printf("is %d < %d? \n",curr_prio,new_job_prio);
			if (curr_prio <= new_job_prio) // if job is larger than curr job, consider next job
			{							   // check if new_job len is less than curr
				prev = curr;
				curr = curr->next;
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
		prev->next = new_job;
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
	if (workload->head == NULL)
	{ // checks if head is NULL
		perror("NULL pointer");
		return -1;
	}
	if (target_id == 1)
	{ // deletes head
		Job *temp = workload->head;
		workload->head = temp->next; // new head is what the next up job
		printf("removed id %d\n", temp->id);
		free(temp); // free meory
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
			printf("curr_id %d\n target_id %d\n",curr->id,target_id);
			if (curr->id == target_id)
			{ // also updates curr_idx
				
				prev->next = curr->next;
				printf("removed id: %d\n", curr->id);
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
		printf("Job ID: %d, Length: %d, Priority: %d\n", curr->id, curr->len, curr->prio);
		curr = curr->next;
	}
	printf("End of Load \n\n");
}

// makes a workload based on the parameter passed into it
// the parameter here should be a file path
int file_to_workload(char path[], Workload *WL, char p_type[])
{

	printf("Priority Type: %s\n", p_type); // print out priority queue type

	// get info from file
	FILE *fp = fopen(path, "r+");
	if (fp == NULL)
	{
		printf("not working");
		return 1;
	}

	// create buffers
	int max_line_size = 255;
	char line[max_line_size]; // line buffer
	int line_nums[2];		  //[0]: length, [1]:priority
	char *token_buff;		  // token (split string) buffer
	int id_buff = 0;		  // used to have id for job ( new job increments the id_buff)

	// fgets(..) = NULL -----> end of a file
	/*
 fgets does the following
 puts first line into buffer
 moves fp to point the next line of the file
 */
	while (fgets(line, max_line_size, fp) != NULL)
	{ // read a line into line, till at the end of file
		int i = 0;
		// get split of line based on delimiter for " "
		// these splits are called tokens
		/*  Example
			if line = "32 3 2";
			then we do token = strtok(line, " ");
			the first split (token) would be "32"
			// strtok interally remembers the position after "32"
			if we do strtok(NULL, " "), then strtok will find the next token for the previously called line
		*/
		// token is a string
		token_buff = strtok(line, " "); // get first token
		// Keep extracting tokens till we have gone through entire line
		while (token_buff != NULL)
		{
			// add number to line_nums
			line_nums[i] = atoi(token_buff); // string -> number
			i++;
			token_buff = strtok(NULL, " "); // get the next token  (strtok remembers where it left off if you use NULL)
		}
		/* at this point, line_nums has the following info
		- the length (0th index)
		- the prioirity (1st index or not there)
		*/
		// create the job
		Job *a_job = create_job(id_buff++, line_nums[0], line_nums[1]); // increment buff job has already been created
		add_job(WL, a_job, p_type);										// add the job to the workload
	}
	fclose(fp);
	// if FIFO, reverse the workload (earliest job is first)
	if ((strcmp(p_type, "FIFO") == 0))
	{
		reverse_workload(WL);
	}
	return 0;
}


int workload_exec(Workload *WL, int time_slice, char p_type[]){
	printf("Execution trace with: %s\n", p_type);

    if(strcmp(p_type, "FIFO") == 0||strcmp(p_type, "SJF") == 0||strcmp(p_type, "PRIO") == 0){
		//printf("I DONT SEE RROBIN \n");
        not_RR_exec(WL);
    } else if(strcmp(p_type, "RoundRobin") == 0) {
		//printf("I SEE RROBIN \n");
        RR_exec(WL, time_slice);
	} else {
		return 1;
	}
	
	printf("End of execution with %s.\n", p_type);
	return 0;
}


// algorithm types that go through the entire linked list and complete jobs in order
// this should be the type required for FIFO, SJF, and PRIO
int not_RR_exec(Workload *WL)
{
	Job *curr = WL->head;
	//int i = 0;
	Job *temp = NULL;
	while (curr != NULL)
	{
		printf("Job %d ran for: %d\n", curr->id, curr->len);
		WL->total_time += curr->len;
		// delete job 
		temp = curr;
		WL->head = curr->next;
		curr = curr->next;
		free(temp);
		//i++;
	}
	return WL->total_time;
}

// executing Round Robin function
// time slice: how much time goes by with each iteration
int RR_exec(Workload *WL, int time_slice)
{
	// printf("Execution trace with RR:\n");
	// Job * prev;
	//  while loop till all WL is empty
	while (WL->head != NULL)
	{
		// go through each job, and remove a certain number
		Job *curr = WL -> head;
		Job *prev = NULL;
		Job *temp = NULL;
		while (curr != NULL)
		{
			// printf("curr->id: %d\ncurr->len: %d\ntime_slice: %d\n\n", curr->id, curr->len, time_slice);
			if (time_slice <= curr->len)
			{
			    // sleep(1); // sleep for 1 second
				printf("Job %d ran for: %d\n", curr->id, time_slice);
				curr->len -= time_slice;	  // remove a time_slice amount of time length from curr job
				WL->total_time += time_slice; // add to total time of workload execution
				
			}
			else
			{								 // the time remaining is less than the time slice
				printf("Job %d ran for: %d\n", curr->id, curr->len);
				curr->len -= curr->len;		 // remove a time_slice amount of time length from curr job
				WL->total_time += curr->len; // add to total time of workload execution
				
			}
			/* if job has  len equal to 0,
				- remove it
				*/
			temp = curr;
			curr = curr->next;
			if (temp->len == 0) 
			{
				
				if (prev != NULL){
					prev -> next = curr;
				} else {// at the head
					// temp = curr;
					WL->head = curr;
					// curr = curr->next;

				}
				// printf("Job %d has been deleted\n",temp -> id);
				free(temp);
			}
			else{
				// if we dont need to delete job, prev shifts to the right
				prev = temp; 
			}
			
			
			
		}
	}
	// printf("End of execution with RR.\n");
	return 0;
}