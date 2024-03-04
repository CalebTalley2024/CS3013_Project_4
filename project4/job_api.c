#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "job_api.h"

/*
	This function is for use with RR_exec(...), and its job is to edit either turnaround time or wait time
	of a Finished_Job_Stats of the specified id.

	Parameters:
	WL_Stats: This is the Workload_Stats struct that contains the information needed for alg_analysis(...)
	id: The int id of the Finished_Job_Stats that we want to change.
	time_type: An integer value that is either 0 or 1. If 0, change turnaround_time of the Finished_Job_Stats
	           that has the specified id.
	value: The number that is being given to the field (aka. what we are editing the field to)

	return: 0 if successful, -1 if error.
*/
int edit_time(Workload_Stats * WL_Stats, int id, int time_type, int value){
	Finished_Job_Stats * curr = WL_Stats->head;
	while(curr != NULL){
		if(curr->id == id){
			if(time_type == 0){
				curr->turnaround_time = value;
				return 0;
			} else if(time_type == 1) {
				curr->wait_time = value;
				return 0;
			} else {
				printf("Time type invalid, use only 0 or 1");
				return -1;
			}
		}
		curr = curr->next;
	}
	printf("Target id not found, could not edit time type %d", time_type);
	return -1;
}

/*
	create_job initializes a new Job

	Parameters:
	id: An int value that represents which Job it is. This will be done in
	len: The length of the Job being creating.
	prio: Sets a priority for the job. This is for the PRIO algorithm if it
	      is used.

	return: a pointer to the Job that was created.
*/
Job *create_job(int id, int len, int prio)
{ // creates job node
	Job *new_job = (Job *)malloc(sizeof(Job));
	// set up pointers
	new_job->id = id;
	new_job->len = len;
	new_job ->init_len = len;
	// new_job->time_spent_waiting = 0; // this will start mattering when the p
	new_job->next = NULL;
	new_job->prio = prio;
	new_job->first_access = 0;
	return new_job;
}

// create job's stats  struct
Finished_Job_Stats *create_job_stats(int id, int res_time, int turnaround_time, int wait_time, int prio){
	Finished_Job_Stats * job_stats = (Finished_Job_Stats *) malloc(sizeof(Finished_Job_Stats));
	
	job_stats -> id = id;
	job_stats -> prio = prio;
	job_stats -> res_time = res_time;
	job_stats -> turnaround_time = turnaround_time;
	job_stats -> wait_time = wait_time;
	return job_stats;
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
	if ((strcmp(p_type, "FIFO") == 0) || (strcmp(p_type, "RR") == 0))
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
				if(workload->head == curr){
					workload->head = new_job;
					new_job->next = curr;
					return 0;
				}
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

/*
	Reverses the order of the Workload_Stats struct given.
*/
int reverse_jobs_stats(Workload_Stats * WL_Stats)
{
	Finished_Job_Stats *head = WL_Stats->head;
	Finished_Job_Stats *prev = NULL, *curr = head, *next_node;

	while (curr != NULL)
	{
		next_node = curr->next; // shift to right
		curr->next = prev;		// point to left
		prev = curr;			// after init to head, shift to right
		curr = next_node;		// shift to right
	}
	// at this point prev is the first job, and curr is null
	WL_Stats->head = prev;
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

// makes a workload and workload stats based on the parameter passed into it
// the parameter here should be a file path
int file_to_workload(char path[], Workload *WL, char p_type[])
{
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

	/* for each line of input....
		- make a job object
		- add job object to workload
		- update workload stats
			- num_jobs
	
	*/
	while (fgets(line, max_line_size, fp) != NULL)
	{ // read a line into line, till at the end of file
		/* Explanation of fgets
			// fgets(..) = NULL -----> end of a file
			fgets does the following
			puts first line into buffer
			moves fp to point the next line of the file
 		*/
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
		// create the job, update num_jobs
		Job *a_job = create_job(id_buff++, line_nums[0], line_nums[1]); // increment buff job has already been created
		add_job(WL, a_job, p_type);	// add the job to the workload
		WL -> stats -> num_jobs++;	

	}
	fclose(fp);
	// if FIFO, reverse the workload (earliest job is first)
	if ((strcmp(p_type, "FIFO") == 0)||(strcmp(p_type, "RR") == 0) )
	{
		reverse_workload(WL);
	}
	return 0;
}

/*
	A helper function that will execute the proper algorithm and give the output desired besed on the tests/#.out template

	Parameters:
	WL: Pointer to the workload we are working with.
	p_type: A string that describes the algorithm we want.
	analysis_only: If this int is 1, that means we only want to show results for algorithm analysis, not the actual
	               result of the algorithm. Also, PRIO ignores this and will always print both the result and analysis.
	prio_stat_map: Pointer to hash map for PRIO implementation.
*/
int workload_exec(Workload *WL, int time_slice, char p_type[], int analysis_only, HashMap *prio_stat_map){
	if(analysis_only == 0 || strcmp(p_type, "PRIO") == 0){ //if we are not doing an analysis test only, print statements are included // also prio is weird
		printf("Execution trace with %s:\n", p_type);
	}

    if(strcmp(p_type, "FIFO") == 0||strcmp(p_type, "SJF") == 0||strcmp(p_type, "PRIO") == 0){
		int is_prio;
		if(strcmp(p_type, "PRIO") == 0){
			is_prio = 1;
			not_RR_exec(WL, 0, is_prio, prio_stat_map);
		} else {
			is_prio = 0;
			not_RR_exec(WL, analysis_only, is_prio, prio_stat_map);
		}
    } else if(strcmp(p_type, "RR") == 0) {
        RR_exec(WL, time_slice);
	} else {
		return 1;
	}
	
	if(analysis_only == 0 || strcmp(p_type, "PRIO") == 0){
		printf("End of execution with %s.\n", p_type);
	}
	return 0;
}


// algorithm types that go through the entire linked list and complete jobs in order
// this should be the type required for FIFO, SJF, and PRIO
int not_RR_exec(Workload *WL, int analysis_only, int is_prio, HashMap *prio_stat_map)
{
	Job *curr = WL->head;
	//int i = 0;
	Job *temp = NULL;
	while (curr != NULL)
	{
		if(analysis_only == 0){
			printf("Job %d ran for: %d\n", curr->id, curr->len);
		}
		int prev_total_time = WL->stats->total_time;
		WL->stats->total_time += curr->len;
		// delete job 
		temp = curr;
		WL->head = curr->next;
		curr = curr->next;
		temp -> T_s = prev_total_time;
		temp -> T_c = WL->stats->total_time;
		float res_time = temp -> T_s;
		float turnaround_time = temp -> T_c;
		float wait_time = temp -> T_s;
		// update the hashmap, this will update the stats for each priority based entry

		//create job stat, add stat to workload stat
		Finished_Job_Stats * new_head = create_job_stats(temp -> id, res_time, turnaround_time, wait_time, temp->prio);
		 
		if(is_prio){ // if the type is prio
			int *prio = &new_head -> prio;
			int *unique_prios = WL -> stats -> unique_prios;
			unique_prios[*prio] = 1;
			// printf("\n\nunique_prios[%d] = 1;\n\n", *prio); 
			hashmap_update(prio_stat_map, prio, &res_time, &turnaround_time, &wait_time);
			new_head -> prio = temp -> prio;
		}
		new_head -> next = WL->stats->head;
		WL->stats->head = new_head;

		free(temp);
	}
	reverse_jobs_stats(WL->stats);
	return 0;
}

// executing Round Robin function
// time slice: how much time goes by with each iteration
int RR_exec(Workload *WL, int time_slice)
{
	//  while loop till all WL is empty
	while (WL->head != NULL)
	{
		// go through each job, and remove a certain number
		Job *curr = WL -> head;
		Job *prev = NULL;
		Job *temp = NULL;
		while (curr != NULL)
		{
			if(curr->first_access == 0){
				curr->first_access = 1;
				int res_time = WL->stats->total_time; //first time being run, acts as response time
				// prio = -1 because we dont care about prio for round robin
				Finished_Job_Stats * new_head = create_job_stats(curr -> id, res_time, 0, 0, -1); //make sure analysis is in order, must create now
				new_head -> next = WL->stats->head; // add to list
		 		WL->stats->head = new_head;
			}
			if (time_slice <= curr->len)
			{
				printf("Job %d ran for: %d\n", curr->id, time_slice);
				curr->len -= time_slice;	  // remove a time_slice amount of time length from curr job
				WL -> stats -> total_time += time_slice; // add to total time of workload execution
			}
			else
			{								 // the time remaining is less than the time slice
				printf("Job %d ran for: %d\n", curr->id, curr->len);
				WL -> stats -> total_time += curr->len; // add to total time of workload execution
				curr->len -= curr->len;		 // remove a time_slice amount of time length from curr job
			}
			/* if job has  len equal to 0,
				- remove it
			*/
			temp = curr;
			curr = curr->next;
			if (temp->len == 0) 
			{
				int wait_time = (WL->stats->total_time) - (temp->init_len);
				edit_time(WL->stats, temp->id, 0, WL->stats->total_time);
				edit_time(WL->stats, temp->id, 1, wait_time);
				if (prev != NULL){
					prev -> next = curr;
				} else {// at the head
					WL->head = curr;
				}
				free(temp);
			}
			else{
				// if we dont need to delete job, prev shifts to the right
				prev = temp; 
			}
		}
	}
	reverse_jobs_stats(WL->stats);
	return 0;
}

/*
	Shows the analysis of the Workload we had and the algorithm type used.

	Parameters:
	WL_Stats: Pointer to the Workload_Stats linked list necessary to retrieve the analysis data.
	p_type: A string of which algorithm was used in completing Jobs.
	prio_stat_map: Pointer to hash map needed for PRIO.
*/
int alg_analysis(Workload_Stats * WL_Stats, char p_type[], HashMap *prio_stat_map){
	printf("Begin analyzing %s:\n", p_type);
	Finished_Job_Stats * curr = WL_Stats -> head;
	//go through WL_stats head, and all nodes. As we iterate, we should
	//print out the job number, res_time, turnaround_time, and wait_time
	while(curr != NULL){
		printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", curr -> id, curr -> res_time, curr -> turnaround_time, curr -> wait_time);
		WL_Stats -> total_res_time += curr -> res_time;
		WL_Stats -> total_turnaround_time += curr -> turnaround_time;
		WL_Stats -> total_wait_time += curr -> wait_time;
		curr = curr -> next;
	}
	// if we have p_type == prio, we print the avg times for each different priority
	if (strcmp(p_type, "PRIO") == 0){
		// for each job_state node
		// Finished_Job_Stats * curr_stats = WL_Stats->head;

		for (int prio = 1; prio< MAX_UNIQUE_PRIOS; prio++){
			
			int *prio_status = &WL_Stats -> unique_prios[prio];

			if (*prio_status == 1){ // this means that the prio at this iteration is used in the test
				Entry *an_entry = hashmap_get(prio_stat_map, &prio);
				print_entry_avg_time_stats(an_entry);
			}
		}
	}

	float avg_response = (float)(WL_Stats -> total_res_time)/(WL_Stats -> num_jobs);
	float avg_turnaround = (float)(WL_Stats -> total_turnaround_time)/(WL_Stats -> num_jobs);
	float avg_wait = (float)(WL_Stats -> total_wait_time)/(WL_Stats -> num_jobs);

	/* example for float -> int
		int a = 22;
		int b = 7;
		float c = (float)a/b;

		printf("%.2f",c);
		return 0;
	*/

	printf("Average -- Response: %.2f  Turnaround: %.2f  Wait: %.2f\n", avg_response, avg_turnaround, avg_wait);
	printf("End analyzing %s.\n", p_type);

	return 0;
}