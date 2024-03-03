In this directory, you should write the program `scheduler.c` and compile it into the binary `scheduler` using `make`. You should do it inside the Ubuntu machine you have setup, as grading will be done so inside the Ubuntu.

After doing so, you can run the tests from this directory by running the
`./run_tests.sh` script. If all goes well, you will see standard output that indicate that all tests are passed. Run `make test` to run all tests.

Note that the test cases are expecting exact matches in string formatting and whitespace. To confirm if that is the cause of any mismatch, you can change the run_tests.sh program to use "--ignore-all-space" on the lines with the diff command to see if they then pass. If so, you probably have an extra space somewhere, perhaps before the newline character.

Passing all tests are not sufficient to obtain all points for this assignment. As an example, any assignment in which hardcodes the expected output to pass the test case will lead to point deductions.


make && ./scheduler

// @caleb notes
// print_workload

// delete(int value, char delete_type) //idx(i) or head(h)

PRIO edge case for printing stats
wait time for completed job w/ Round Robin : total _time - initial_job_length
Policy Analysis: Novel Workloads

---------------------------------------DATA STRUCTURES---------------------------------------
We used a linked list implementation for creating workloads. We have a struct that is called
Workload and one that is called job_node. The Workload struct contains the head of the linked
list, which is a job_node. The Workload struct also has another struct called Workload_Stats.
The purpose of the Workload_Stats struct is to track anything that will be useful for
analysis of the Workload algorithms used. The Workload_Stats struct is, just like the
Workload struct, a linked list. The node struct for Workload_Stats is called job_stats. Now
that the explanation of how the linked lists are made is done, further explanation is needed
to explain the different fields of each struct.

typedef struct Workload{
    Job *head;

	Workload_Stats *stats;
}Workload;

head: A pointer to the head of the workload. This is the first job.
Stats: A pointer to a Workload_Stats struct that will be used for future analysis.


typedef struct job_node { // jobNode
		int id;
		int len; //length/amount of time to complete the job
		int prio; // job's priority  // default val = 0
		int T_s; // T(starts)
		int T_c; // T(completes)
		int init_len; // initial length of job used for RR
		int first_access; //used to track when to store response time for RR
		struct job_node*next;
}Job;

id:   Contains the id of the job. As jobs are made, an id will be given to them that is
      one more than the previous job (eg. 0,1,2,3,4,5,6,NULL).
len:  The amount of simulated time that the job has left to finish.
prio: The priority of the job. This is only important when the PRIO algorithm is
         being used.
T_s:  Contains the time that the job was first accessed. In other words, this is the
      response time of this particular job.
T_c:  Contains the time that it is when the job finishes. Basically, it's the turnaround time.
init_len: The initial length of the job. This is important for calculating wait time.
first_access: When a job is created, this is initialized to 0. Then, when we are doing the
              round robin approach, we can create job_stats in the order that the jobs come
              in, rather than as the jobs are deleted (which could cause issues with order).
              This value is changed to 1 once the job_stats for that job is created in the
              function.
next: A pointer to the next job in the linked list. The end of the list will be NULL.


typedef struct workload_stats{
	Finished_Job_Stats *head;
	int T; // workload initial time 
	int total_time;
	int total_res_time;
	int total_wait_time;
	int total_turnaround_time;
	int num_jobs;
}Workload_Stats;

head: A pointer to the first job_stats of the linked list.
T: For the purposes of this homework assignment, this value will always be 0.
total_time: The total amount of time that has elapsed when using an algorithm. This value
            is important for making calculations for response time, turnaround time, and wait
            time.
total_res_time: Contains the total response time for the entire workload. Used for
                calculating averages in the analysis of algorithms.
total_wait_time: The same as total_res_time but for wait times.
total_turnaround_time: The same as total_res_time but for turnaround time.
num_jobs: Contains the total number of jobs. This is used for calculating the averages of
          response time, wait time, and turnaround time.


typedef struct job_stats{
	int id;
	int res_time;
	int turnaround_time;
	int wait_time;
	struct job_stats *next;
}Finished_Job_Stats;

id: Contains the id that corresponds to the job that this struct is storing stats for.
res_time: Stores the response time of the job. This is done when the job finishes.
turnaround_time: Same as res_time, but for turnaround time.
wait_time: Same as res_time, but for wait time.
next: A pointer to the next job_stats node.


typedef struct Entry{
    int * num_jobs_completed;
    int * prio;
    int * total_res_time;
    int * total_turnaround_time;
    int * total_wait_time;
    struct Entry *next;
}Entry;

stuff


typedef struct HashMap {
    Entry **table;
} HashMap;

stuff
---------------------------------------------------------------------------------------------





---------------------------------------NOVEL WORKLOADS---------------------------------------
1: For our first novel workload we have 5 jobs of length 4. Having 5 equally sized jobs 
   that are multiples of 4 causes each job to finish upon being reached for the first time.
   This effectively makes each job's response time and wait time the same.
2: For our second novel workload we have a job of length 200 followed by 19 jobs of
   length 1. After analysis of FIFO and SJF, the resulting average turnaround time was
   209.5 and 20.45 respectively (roughly a 10 times difference). The FIFO algorithm needs
   to finish the long job first, making the turnaround time of the smaller jobs suffer,
   while the SJF algorithm lets the shorter jobs have lower turnaround times because they
   did not have to wait on the long job.
3: Much like the first novel workload, the third has 5 jobs that are all length 4. 
   Having 5 equally sized jobs that are multiples of 4 allows the RR algorithm to run the
   same as FIFO and SJF. SJF and FIFO will be exactly the same in this case because every
   job is the same length.
4: For our fourth novel workload we have 5 jobs. The first four jobs are length 1, and the
   fifth job is length 1000. Upon analyzing, the average turnaround time was 202.8 and the
   average wait time was 2.
5: Novel workload five has three jobs of length 3, 9, and 12. After analyzing the FIFO
   algorithm with this workload, the average response time was 5 and the average turnaround
   time was 13.
---------------------------------------------------------------------------------------------

Make sure all test cases 
Make sure novel workloads are in correct folder
