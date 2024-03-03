#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hashmap entry
typedef struct Entry{
    char *key;
    void *value;
    struct Entry *next;
}Entry;

typedef struct HashMap {

    Entry **table; //@ caleb: why use two point4ers
}HashMap;



HashMap* create_hashmap(num_entries) {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL){return NULL;}
    
    map ->table = (Entry **)calloc(num_entries,sizeof(Entry*));
    if (map ->table == NULL){
        free(map);
        return NULL;
    }
    return map;



}