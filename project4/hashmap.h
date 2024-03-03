#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_ENTRIES 1000

typedef struct Entry {
    float *num_jobs_completed;
    int *prio;
    float *total_res_time;
    float *total_turnaround_time;
    float *total_wait_time;
    struct Entry *next;
}Entry;

typedef struct HashMap {
    Entry **table;
}HashMap;

unsigned long hash(int *prio);
HashMap *create_hashmap(int num_entries);
int hashmap_update(HashMap *map, int *prio, float *res_time, float *turnaround_time, float *wait_time);
Entry *hashmap_get(HashMap *map, int *prio);
void free_hashmap(HashMap *map);
int print_entry_avg_time_stats(Entry *entry);

#endif // HASHMAP_H
