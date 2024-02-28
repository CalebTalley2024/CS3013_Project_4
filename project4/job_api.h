
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Helper macro for quick error checking.




typedef struct job_node { // jobNode
		int id ;
		int len; //length/amount of time to complete the job
		int prio; // priority // default val = 0
		struct job_node *next;
		int time_spent_waiting;
		//should we have another variable that constantly increases called "totalTime"
		//when the job enters the scheduler, this variable should increase until the job is being worked on

		//len tracks the amt  of time needed to finish the job
		//Ill explain via example. Let's say we have a job running. len should be subtracted by one in a while loop,
		//While this happens, 1 should be added to any other job that is waiting (not to len, but to another variable)
		//in this way, we can accurately track both the time it takes to finish a given job, and the total time it took
		//to finish it.
		
		/*
		we track the wait time for a job to start running?
	
		*/
		//while(len != 0){
			//len--;
			//other_variable++; (for every other job)
		//}

		// keeping track the amount of time we have ran for each job in the workload?

		//we need to track the total amount of time that this job has existed. right? This variable would allow us to
		// I was thinking that we woould just subtract from len. 
		// Maybe youre way is better depending on what we need for Part 2 of the project. Yeah. we need to be able to
		// get the total time of the program. So having a variable for when the project is not being worked on would be nice.
		// like total time for the workload, or for an individual job
		// other meta data
		
}Job;


typedef struct job_list{
    Job *head;
}Workload;


Job *create_job(int id, int len, int prio);
int add_job(Workload *workload, Job *new_job, const char type[]);
int delete_job(Workload *workload, int target_id);
int reverse_workload(Workload *workload);
void print_workload(Workload *workload);