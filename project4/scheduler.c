#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job_api.h"



// Job file_to_job(){

//     // get the disk
//     disk = fopen("disk.txt", "r+")
//     if ( disk!= NULL){return -1;} // check that disk is not NULL



// }


int main(int argc, char **argv) { 


    // Array to store the numbers (adjust size as needed)
    // print_workload(workload);
    char p_type[] = "RoundRobin";
    Workload *workload = (Workload *)malloc(sizeof(Workload));
    char path[] = "tests/18.in";

    workload -> total_time = 0;

    file_to_workload(path, workload, p_type);

    print_workload(workload);

    int time_slice = 10;
    
    workload_exec(workload, time_slice, p_type);

    free(workload);

    return 0;
}
