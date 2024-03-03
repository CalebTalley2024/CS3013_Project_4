
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Helper macro for quick error checking.
typedef struct job_node { // jobNode
		int id;
		int len; //length/amount of time to complete the job
		int prio; // priority // default val = 0
		struct job_node*next;
		int T_s; // T(starts)
		int T_c; // T(completes)

}Job;




typedef struct job_stats{
	int id;
	int res_time;
	int turnaround_time;
	int wait_time;
	struct job_stats *next;
}Finished_Job_Stats;


typedef struct workload_stats{
	Finished_Job_Stats *head;
	int T; // workload initial time 
	int total_time;
	// use to get avg stats
	int total_res_time;
	int total_wait_time;
	int total_turnaround_time;
	int num_jobs;
}Workload_Stats;

typedef struct Workload{
    Job *head;

	Workload_Stats *stats;
}Workload;


Job *create_job(int id, int len, int prio);
Finished_Job_Stats *create_job_stats(int id, int res_time, int turnaround_time, int wait_time);
int add_job(Workload *workload, Job *new_job, const char type[]);
int reverse_workload(Workload *workload);
void print_workload(Workload *workload);
int file_to_workload(char path[], Workload* WL, char p_type[]);
int workload_exec(Workload *WL, int time_slice, char p_type[]);
int not_RR_exec(Workload * workload);
int RR_exec(Workload *WL, int time_slice);
int alg_analysis(Workload_Stats * WL_Stats, char p_type[]);
int reverse_jobs_stats(Workload_Stats * WL_Stats);