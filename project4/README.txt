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

---------------------------------------------NOVEL WORKLOADS----------------------------------------------
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

Make sure all test cases 
Make sure novel workloads are in correct folder
