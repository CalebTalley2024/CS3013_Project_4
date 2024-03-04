#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job_api.h"

int main(int argc, char **argv) { 

    if(argc != 4){ //error handling
        printf("This program requires 4 arguments:\n");
        printf("The first argument is ./scheduler\nThe second is the algorithm type\nThe third is the test file\nAnd the fourth is the time slice\n");
        return -1;
    }

    char p_type[strlen(argv[1])];
    strcpy(p_type, argv[1]);

    Workload *WL = (Workload *)malloc(sizeof(Workload));
    Workload_Stats *WL_stats = (Workload_Stats *)malloc(sizeof(Workload_Stats));
    WL -> stats = WL_stats; // link stats
    char path[strlen(argv[2])];
    strcpy(path, argv[2]);

    // explicit init of total_time and initital time T
    WL -> stats -> total_time = 0;
    WL -> stats -> T = 0;

    file_to_workload(path, WL, p_type);

    int time_slice = atoi(argv[3]);
    int alg_or_not;
    
    char  ones_digit = path[7]  - '0'; // '.' - '0' = 46 - 48 = -2, 
    if(ones_digit < 0 || ones_digit == 1 || ones_digit == 0){
        alg_or_not = 0;
    } else {
        alg_or_not = 1;
    }
 
    // create hashmap
    // key: priority
    // values: time stats ( response, turnaround, wait)
    HashMap *prio_stat_map = create_hashmap(WL->stats->num_jobs);

    workload_exec(WL, time_slice, p_type, alg_or_not, prio_stat_map);
    //depending on what alg_or_not is, the workload_exec will either have
    //print statements, or avoid them. For 1, it will avoid, and for 0, it
    //will include.

    if(alg_or_not == 1){
        alg_analysis(WL -> stats, p_type, prio_stat_map);
    }

    free(WL);

    return 0;
}