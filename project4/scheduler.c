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
    char p_type[] = "FIFO";
    Workload *WL = (Workload *)malloc(sizeof(Workload));
    Workload_Stats *WL_stats = (Workload_Stats *)malloc(sizeof(Workload_Stats));
    WL -> stats = WL_stats; // link stats
    char path[] = "tests/1.in";
    //char path[] = "novel_workloads/workload_2.in";

    // explicit init of total_time and initital time T
    WL -> stats -> total_time = 0;
    WL -> stats -> T = 0;

    file_to_workload(path, WL, p_type);

    // print_workload(workload);

    int time_slice = 5;
    
    workload_exec(WL, time_slice, p_type);

    alg_analysis(WL -> stats, p_type);

    free(WL);

    return 0;
}


//3 9