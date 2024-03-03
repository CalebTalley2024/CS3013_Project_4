#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "job_api.h"

//if 0, edit turnaround time, if 1, edit wait time
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
Finished_Job_Stats *create_job_stats(int id, int res_time, int turnaround_time, int wait_time){
	Finished_Job_Stats * job_stats = (Finished_Job_Stats *) malloc(sizeof(Finished_Job_Stats));
	
	job_stats -> id = id;
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

	//printf("Priority Type: %s\n", p_type); // print out priority queue type

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

	//printf("The total # of Jobs %d\n", WL -> stats -> num_jobs);
	return 0;
}


int workload_exec(Workload *WL, int time_slice, char p_type[], int analysis_only){
	if(analysis_only == 0){ //if we are not doing an analysis test only, print statements are included
		printf("Execution trace with %s:\n", p_type);
	}

    if(strcmp(p_type, "FIFO") == 0||strcmp(p_type, "SJF") == 0||strcmp(p_type, "PRIO") == 0){
		//printf("I DONT SEE RROBIN \n");
        not_RR_exec(WL, analysis_only);
    } else if(strcmp(p_type, "RR") == 0) {
		//printf("I SEE RROBIN \n");
        RR_exec(WL, time_slice);
	} else {
		return 1;
	}
	
	if(analysis_only == 0){
		printf("End of execution with %s.\n", p_type);
	}
	return 0;
}


// algorithm types that go through the entire linked list and complete jobs in order
// this should be the type required for FIFO, SJF, and PRIO
int not_RR_exec(Workload *WL, int analysis_only)
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
		int res_time = temp -> T_s;
		int turnaround_time = temp -> T_c;
		int wait_time = temp -> T_s;

		// if(WL -> stats -> head == NULL){
		// 	WL->stats->head = create_job_stats(temp -> id, res_time, turnaround_time, wait_time);
		// } else {
		//create job stat, add stat to workload stat
		 Finished_Job_Stats * new_head = create_job_stats(temp -> id, res_time, turnaround_time, wait_time);
		 new_head -> next = WL->stats->head;
		 WL->stats->head = new_head;
		//}

		free(temp);
		//i++;
	}
	reverse_jobs_stats(WL->stats);
	return WL->stats->total_time;
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
			if(curr->first_access == 0){
				curr->first_access = 1;
				int res_time = WL->stats->total_time; //first time being run, acts as response time
				Finished_Job_Stats * new_head = create_job_stats(curr -> id, res_time, 0, 0); //make sure analysis is in order, must create now
				new_head -> next = WL->stats->head; // add to list
		 		WL->stats->head = new_head;
			}
			// printf("curr->id: %d\ncurr->len: %d\ntime_slice: %d\n\n", curr->id, curr->len, time_slice);
			if (time_slice <= curr->len)
			{
			    // sleep(1); // sleep for 1 second
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
				//printf("Total time: %d\n", WL->stats->total_time);
				edit_time(WL->stats, temp->id, 0, WL->stats->total_time);
				edit_time(WL->stats, temp->id, 1, wait_time);
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
	reverse_jobs_stats(WL->stats);
	// printf("End of execution with RR.\n");
	return 0;
}

int alg_analysis(Workload_Stats * WL_Stats, char p_type[]){
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
		//printf("Number of Jobs: %d\n", WL_Stats -> num_jobs);
	}
	
	float avg_response = (float)(WL_Stats -> total_res_time)/(WL_Stats -> num_jobs);
	float avg_turnaround = (float)(WL_Stats -> total_turnaround_time)/(WL_Stats -> num_jobs);
	// printf("Total Wait: %d\n", WL_Stats -> total_wait_time);
	// printf("Number of Jobs: %d\n", WL_Stats -> num_jobs);
	float avg_wait = (float)(WL_Stats -> total_wait_time)/(WL_Stats -> num_jobs);

	/*
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

//wait time
//response time is the first time it starts working
//turnaround time how long it took since time = 0