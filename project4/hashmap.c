#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "hashmap.h"

#define MAX_NUM_ENTRIES 1000 // maximum size of hash table

// Key: prio
// basic hashing function
unsigned long hash(int *prio){
    return *prio % MAX_NUM_ENTRIES;
}

// create a hashmap
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
        if (*(existing_entry->prio) == *prio){
            // update total times
            *existing_entry->total_res_time += *res_time;
            *existing_entry->total_turnaround_time += *turnaround_time;
            *existing_entry->total_wait_time += *wait_time;

            (*existing_entry->num_jobs_completed)++; // add to the amount of completed jobs
            return 0;
        }
    }

    // If entry for prio not found, make new entry
    // allocate memory
    Entry *new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->prio = malloc(sizeof(int));
    new_entry->total_res_time = malloc(sizeof(float));  // Allocate memory for the new entry
    new_entry->total_turnaround_time = malloc(sizeof(float));
    new_entry->total_wait_time = malloc(sizeof(float));
    new_entry->num_jobs_completed = malloc(sizeof(int));

    // initialize pointers with parameter time values
    *new_entry->prio = *prio;
    *new_entry->total_res_time = *res_time;
    *new_entry->total_turnaround_time = *turnaround_time;
    *new_entry->total_wait_time = *wait_time;
    *new_entry->num_jobs_completed = 1;

    // make the table pointer at idx prio point to new_entry
    map->table[idx] = new_entry;
    return 0;
}


// get Entry struct using the priority/key
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

// prints the average response time, turnaround time, and wait time for each separate priority level
int print_entry_avg_time_stats(Entry * entry){
    if (entry == NULL) {
        perror("Error: Entry is NULL\n");
        return -1;  // Or handle the error in an appropriate way
    }
    float num_jobs_prio = *(entry ->num_jobs_completed);
    // calculate averages
    float avg_total_res_time = *(entry->total_res_time)/num_jobs_prio;
    float avg_total_turnaround_time = *(entry->total_turnaround_time) / num_jobs_prio;
    float avg_total_wait_time = *(entry->total_wait_time) / num_jobs_prio;
    printf("Priority %d: Average -- Response: %0.2f  Turnaround: %0.2f  Wait: %0.2f\n",
           *entry -> prio, avg_total_res_time, avg_total_turnaround_time, avg_total_wait_time);
    return 0;
}