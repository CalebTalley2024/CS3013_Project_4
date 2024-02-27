#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Helper macro for quick error checking.
#define CHECK(x)	do { if (!(x)) { fprintf(stderr, "%s:%d CHECK failed: %s, errno %d %s\n", __FILE__, __LINE__, #x, errno, strerror(errno)); abort(); } } while(0)

typedef struct job_node { // jobNode
		int id ;
		int length ;
		// other meta data
		struct jobNode *next ;
}Job;


typedef struct job_list{
    Job *head;
}Workload;


Job *create_job(int id, int length){ // creates job node
    Job *new_job = (Job *)malloc(sizeof(Job));
	CHECK(new_job); // make sure job != NULL

	// set up pointers
	new_job -> id = id;
	new_job -> length = length;
	new_job -> next = NULL;
	return new_job;
}

void add_job(Workload *workload, int id, int length){ // by default, adds job to the head of the list
	Job  * new_job = create_job(id, length);
	new_job -> next = workload -> head;
	workload -> head = new_job;
}

remove_job(Workload *workload, int target_id){ // removes job based on id

	Job *curr_job = workload -> head;
	Job *prev_job = NULL;

	while (curr_job != NULL){ // check where the job is 

		// if target job is head
		if (workload->head->id == target_id){

		}

		// if not head
	}
}


