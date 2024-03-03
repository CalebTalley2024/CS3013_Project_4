#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job_api.h"
// #include "hashmap.h"



// Job file_to_job(){

//     // get the disk
//     disk = fopen("disk.txt", "r+")
//     if ( disk!= NULL){return -1;} // check that disk is not NULL



// }


int main(int argc, char **argv) { 
    
    // printf("%d",argv[0]);
    // printf("%d",argv[1]);
    // printf("%d",argv[2]);
    // printf("%d",argv[3]);
    if(argc != 4){
        printf("This program requires 4 arguments:\n");
        printf("The first argument is ./scheduler\nThe second is the algorithm type\nThe third is the test file\nAnd the fourth is the time slice\n");
        return -1;
    }


    // Array to store the numbers (adjust size as needed)
    // print_workload(workload);
    //char p_type[] = "FIFO";
    //printf("%s", argv[1]);
    char p_type[strlen(argv[1])];
    strcpy(p_type, argv[1]);

    Workload *WL = (Workload *)malloc(sizeof(Workload));
    Workload_Stats *WL_stats = (Workload_Stats *)malloc(sizeof(Workload_Stats));
    WL -> stats = WL_stats; // link stats
    //char path[] = "tests/1.in";
    //char path[] = "novel_workloads/workload_2.in";
    char path[strlen(argv[2])];
    strcpy(path, argv[2]);

    // explicit init of total_time and initital time T
    WL -> stats -> total_time = 0;
    WL -> stats -> T = 0;

    file_to_workload(path, WL, p_type);

    // print_workload(workload);

    int time_slice = atoi(argv[3]);
    int alg_or_not;
    
    //printf("%s\n", path);
    char  ones_digit = path[7]  - '0'; // '.' - '0' = 46 - 48 = -2, 
    if(ones_digit < 0 || ones_digit == 1 || ones_digit == 0){
        alg_or_not = 0;
    } else {
        alg_or_not = 1;
    }
    //printf("Digit 1: %d\nDigit 2?: %d\n", digit1, maybedigit2);
    //char is_analysis = path[5];
    //printf("Character at index 8: %s\n", is_analysis);
 
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


//3 9

// int main(int argc, char **argv) { 
//     for (int i = 0; i< argc; i++){
//     printf("%s\n", argv[i]);
//     }

//     char test[strlen(argv[1])];

//     strcpy(test, argv[1]);
//     printf("%s\n", test);
// }