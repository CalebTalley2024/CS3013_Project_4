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

    // get info from file
    FILE *fp = fopen("temp.txt", "r+");
    if (fp == NULL){ printf("not working"); return 1;} 

    int max_line_size = 255;
    char line[max_line_size]; // line buffer
    /* 
     fgets does the following
     puts first line into buffer
     moves fp to point the next line of the file
     */
    if (fgets(line, max_line_size, fp) == NULL){ printf("not working"); return 1;}
    // printf("%c\n",line[0]);
    fclose(fp);
    // Array to store the numbers (adjust size as needed)
    int line_nums[2]; //[0]: length, [1]:priority
    char *token_buff; // token (split string) buffer
    int i = 0;
    // get split of line based on delimiter for " "
    // these splits are called tokens
    /*  Example
        if line = "32 3 2";
        then we do token = strtok(line, " ");
        the first split (token) would be "32"
        // strtok interally remembers the position after "32"
        if we do strtok(NULL, " "), then strtok will find the next token for the previously called line
    */
    // token is a string 
    token_buff = strtok(line, " "); // get first token
    // Keep extracting tokens while they exist
    while (token_buff != NULL) {
        line_nums[i] = atoi(token_buff);  // string -> number
        i++;
        token_buff = strtok(NULL, " "); // get the next token
    }

    // Print the resulting array
    for (int j = 0; j < i; j++) {
        printf("%d\n", line_nums[j]);
    }
    printf("\n");

    // for (int i  = 0; i< argc; i++){
    //     printf( "argv[%d]: %s\n",i,argv[i]);
    // }
    // argv[1]: file path


    // Workload *W1 = (Workload *)malloc(sizeof(Workload));

    // printf("Hello, please help me schedule!\n\n");

    // Job *j1 = create_job(1, 4, 1);
    // Job *j2 = create_job(2, 14, 3);
    // Job *j3 = create_job(3, 204, 5);
    // Job *j4 = create_job(4, 34, 2);

    // (1) FIFO, (2) SJF, (3) RoundRobin, (4) PRIO");

    // scanf("%d", &p_type_int);

    // char p_type[] = "PRIO";
    // char p_type[] = "SJF"; // DOES NOT WORK
    // char p_type[] = "RoundRobin";
    // char p_type[] = "PRIO"; // DOES NOT WORK
    // printf("Priority Type: %s\n", p_type);

    // add_job(W1, j1, p_type);
    // add_job(W1, j2, p_type);
    // add_job(W1, j3, p_type);
    // add_job(W1, j4, p_type);

    //fscanf

    // if ((strcmp(p_type, "FIFO") == 0) || (strcmp(p_type, "RoundRobin") == 0)) {
    //     reverse_workload(W1);
    // }

    
    // printf("Before Delete\n");
    // print_workload(W1);
    // delete_job(W1,1);
    // printf("After Delete\n");

    // print_workload(W1);

    // Don't forget to free allocated memory before exiting the program
    // free(W1);

    return 0;
}
