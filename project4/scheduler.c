#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job_api.h"

int main(int argc, char **argv) {
    Workload *W1 = (Workload *)malloc(sizeof(Workload));

    printf("Hello, please help me schedule!\n\n");

    Job *j1 = create_job(1, 4, 1);
    Job *j2 = create_job(2, 14, 3);
    Job *j3 = create_job(3, 204, 5);
    Job *j4 = create_job(4, 34, 2);

    // (1) FIFO, (2) SJF, (3) RoundRobin, (4) PRIO");

    // scanf("%d", &p_type_int);

    char p_type[] = "PRIO";
    // char p_type[] = "SJF"; // DOES NOT WORK
    // char p_type[] = "RoundRobin";
    // char p_type[] = "PRIO"; // DOES NOT WORK
    printf("Priority Type: %s\n", p_type);

    add_job(W1, j1, p_type);
    add_job(W1, j2, p_type);
    add_job(W1, j3, p_type);
    add_job(W1, j4, p_type);

    //fscanf

    if ((strcmp(p_type, "FIFO") == 0) || (strcmp(p_type, "RoundRobin") == 0)) {
        reverse_workload(W1);
    }

    
    // printf("Before Delete\n");
    print_workload(W1);

    // delete_job(W1,1);

    // printf("After Delete\n");

    // print_workload(W1);

    // Don't forget to free allocated memory before exiting the program
    free(W1);

    return 0;
}
