#define MAX_NUM_ENTRIES 1000 // maximum size of hash table
// hashmap entry
typedef struct Entry{
    int *key;
    void *value;
    struct Entry *next;
}Entry;

typedef struct HashMap {
    Entry **table; //@ caleb: why use two pointers
}HashMap;


unsigned long hash(int *key){
    return *key % MAX_NUM_ENTRIES;
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



// set a new value for hashmap
int hashmap_set(HashMap *map, int *key, void *value){
    // key --> idx
    unsigned long idx = hash(key);
    // get the entry * at the location of the idx
    Entry * existing_entry = map->table[idx];
    while (existing_entry != NULL){
        // check if an entry that shoudl have a certain key is shifted
        // this would happen if there were multiple entries with the same key
        if (existing_entry->key == key){
            existing_entry -> value = value;
            return 0;
        }
        existing_entry = existing_entry->next;
    }
    // If key not found, make new entry
    Entry *new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry -> key = malloc(sizeof(int));
    *(new_entry -> key) = *key; // Copy the value of the key
    new_entry->value = value;
    // if key not found, slide this new entry ahead of the entry at table[idx]
    new_entry->next = map-> table[idx];
    map->table[idx] = new_entry;
    return 0;
}

// get value at the key
void* hashmap_get(HashMap *map, int *key){
    unsigned long idx = hash(key);
    Entry *entry = map -> table[idx];
    // find the entries value
    while (entry != NULL){
        if (*entry -> key == *key){
            return entry -> value;
        }
        entry = entry ->next;
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