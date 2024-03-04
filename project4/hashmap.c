#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "hashmap.h"

#define MAX_NUM_ENTRIES 1000 // maximum size of hash table
// hashmap entry
// typedef struct Entry{
//     int * num_jobs_completed;
//     int * prio;
//     int * total_res_time;
//     int * total_turnaround_time;
//     int * total_wait_time;
//     struct Entry *next;
// }Entry;

// typedef struct HashMap {
//     Entry **table; //@ caleb: why use two pointers
// }HashMap;



// Key: prio
unsigned long hash(int *prio){
    return *prio % MAX_NUM_ENTRIES;
}

HashMap* create_hashmap(int num_entries) {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL){return NULL;}

    map ->table = (Entry **)calloc(num_entries,sizeof(Entry*));
    if (map ->table == NULL){
        free(map);
        return NULL;
    }
    return map;
}



// updates/add the values of an entry given a prio (the priority)
int hashmap_update(HashMap *map, int *prio, float * res_time,float * turnaround_time, float * wait_time){
    // prio --> idx
    unsigned long idx = hash(prio);
    // get the entry * at the location of the idx
    Entry * existing_entry = map->table[idx];
    while (existing_entry != NULL){
        // check if an entry that should have a certain prio is shifted
        // this would happen if there were multiple entries with the same prio
        if (*(existing_entry->prio) == *prio){
            // printf("    prio: %d\n",*prio);
            // printf("    res_time: %0.2f\n", *res_time);
            // printf("    turnaround_time: %0.2f\n", *turnaround_time);
            // printf("    wait_time: %0.2f\n", *wait_time);

            // printf("        total_res_time: %0.2f\n", *existing_entry->total_res_time);
            // printf("        total_turnaround_time: %0.2f\n", *existing_entry->total_turnaround_time);
            // printf("        total_wait_time: %0.2f\n", *existing_entry->total_wait_time);
            *existing_entry->total_res_time += *res_time;
            *existing_entry->total_turnaround_time += *turnaround_time;
            *existing_entry->total_wait_time += *wait_time;
            // printf("        total_res_time: %0.2f\n", *existing_entry->total_res_time);
            // printf("        total_turnaround_time: %0.2f\n", *existing_entry->total_turnaround_time);
            // printf("        total_wait_time: %0.2f\n", *existing_entry->total_wait_time);

            (*existing_entry->num_jobs_completed)++; // add to the amount of completed jobs
            return 0;
        }
        // existing_entry = existing_entry->next;
    }

    // printf("New entry created for prio: %d\n",*prio);
    // If prio not found, make new entry
    Entry *new_entry = (Entry*)malloc(sizeof(Entry));

    new_entry->prio = malloc(sizeof(int));
    new_entry->total_res_time = malloc(sizeof(float));  // Allocate memory for the new entry
    new_entry->total_turnaround_time = malloc(sizeof(float));
    new_entry->total_wait_time = malloc(sizeof(float));
    new_entry->num_jobs_completed = malloc(sizeof(int));

    *new_entry->prio = *prio;
    *new_entry->total_res_time = *res_time;
    *new_entry->total_turnaround_time = *turnaround_time;
    *new_entry->total_wait_time = *wait_time;
    // if prio not found, slide this new entry ahead of the entry at table[idx]
    new_entry->next = map-> table[idx];
    map->table[idx] = new_entry;
    new_entry->num_jobs_completed = malloc(sizeof(int)); // add to the amount of completed jobs
    *(new_entry->num_jobs_completed) = 1;


    // printf("    prio: %d\n",*prio);
    // printf("    res_time: %0.2f\n", *res_time);
    // printf("    turnaround_time: %0.2f\n", *turnaround_time);
    // printf("    wait_time: %0.2f\n", *wait_time);
    // printf("        total_res_time: %0.2f\n", *new_entry->total_res_time);
    // printf("        total_turnaround_time: %0.2f\n", *new_entry->total_turnaround_time);
    // printf("        total_wait_time: %0.2f\n", *new_entry->total_wait_time);
    return 0;
}


// get entry
Entry* hashmap_get(HashMap *map, int *prio){
    unsigned long idx = hash(prio);
    Entry *entry = map -> table[idx];
    // find the entries value
    while (entry != NULL){
        if (*entry -> prio == *prio){
            return entry;
        }
        entry = entry ->next;  // dont find it? look at the next one
    }
    return NULL;
}

// free memory from hashmap
void free_hashmap(HashMap *map){
    // remove entry memory
    for (int i = 0; i< MAX_NUM_ENTRIES; i++){
        Entry *entry = map -> table[i];
        while (entry != NULL){
            Entry *next  = entry -> next;
            free (entry);
            entry = next;
        }
    }
    free(map -> table);
    free(map);
}

int print_entry_avg_time_stats(Entry * entry){
    if (entry == NULL) {
        perror("Error: Entry is NULL\n");
        return -1;  // Or handle the error in an appropriate way
    }
    float num_jobs_prio = *(entry ->num_jobs_completed);
    float avg_total_res_time = *(entry->total_res_time)/num_jobs_prio;
    float avg_total_turnaround_time = *(entry->total_turnaround_time) / num_jobs_prio;
    float avg_total_wait_time = *(entry->total_wait_time) / num_jobs_prio;
    printf("Priority %d: Average -- Response: %0.2f  Turnaround: %0.2f  Wait: %0.2f\n",
           *entry -> prio, avg_total_res_time, avg_total_turnaround_time, avg_total_wait_time);
    return 0;
}