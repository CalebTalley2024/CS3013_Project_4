#ifndef JOB_API_H
#define JOB_API_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "hashmap.h"

#define MAX_UNIQUE_PRIOS 10// test 18 has 3 different prios, we just add 10 just in case



// Helper macro for quick error checking.
typedef struct job_node { // jobNode
		int id;
		int len; //length/amount of time to complete the job
		int prio; // priority // default val = 0
		int T_s; // T(starts)
		int T_c; // T(completes)
		int init_len; // initial length of job used for RR
		int first_access; //used to track when to store response time for RR
		struct job_node*next;
}Job;

//struct that tracks each jobs' stats. This is important for analysis of algorithms.
typedef struct job_stats{
	int id;
	int prio;
	int res_time;
	int turnaround_time;
	int wait_time;
	struct job_stats *next;
}Finished_Job_Stats;

//Has the beginning of a linked list of job_stats, and also keeps track of "total" values
typedef struct workload_stats{
	Finished_Job_Stats *head;
	int T; // workload initial time 
	int total_time;
	// use to get avg stats
	int total_res_time;
	int total_wait_time;
	int total_turnaround_time;
	int num_jobs;
	int unique_prios[MAX_UNIQUE_PRIOS];// list of all of the unique priorities
	/*
	unique_prios[1] = 1

	there exists a priority 1
	unique_prios[0] = 0

	there d.n.e a pirority 0
	*/
}Workload_Stats;

//This contains the start of the linked list of Jobs, and also includes a Workload_Stats struct.
typedef struct Workload{
    Job *head;
	Workload_Stats *stats;
}Workload;

/*

hashmap to store prio values

array to get the order of job execution

init hashmap with array

go through list and update hashmap valoues

print for each hashmap key

*/


Job *create_job(int id, int len, int prio);
Finished_Job_Stats *create_job_stats(int id, int res_time, int turnaround_time, int wait_time, int prio);
int add_job(Workload *workload, Job *new_job, const char type[]);
int reverse_workload(Workload *workload);
void print_workload(Workload *workload);
int file_to_workload(char path[], Workload* WL, char p_type[]);
int workload_exec(Workload *WL, int time_slice, char p_type[], int alg_only, HashMap *prio_stat_map);
int not_RR_exec(Workload * workload, int only_alg, int is_prio, HashMap *prio_stat_map);
int RR_exec(Workload *WL, int time_slice);
int alg_analysis(Workload_Stats * WL_Stats, char p_type[], HashMap *prio_stat_map);
int reverse_jobs_stats(Workload_Stats * WL_Stats);
int edit_time(Workload_Stats * WL_Stats, int id, int time_type, int value);

#endif // JOB_API_H