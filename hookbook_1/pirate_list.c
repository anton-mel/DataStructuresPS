// LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// HEADERS
#include "pirate_list.h"

// GLOBAL
#define INITIAL_CAPACITY 25 // it's better be 2 to some power
#define RESIZE_FACTOR 2

// PIRATE LST IMPLEMENTATION
struct implementation {
    size_t cur_size;
    size_t max_size;
    pirate **pirates;
};

/////////////////////////////////////////////////////
// PIRATE LIST FUNCTIONS ////////////////////////////
/////////////////////////////////////////////////////

// EXPAND PIRATE LIST
void expandIfNecessary (pirate_list *pirates) {
    if (pirates->cur_size >= pirates->max_size) {
        pirates->max_size *= RESIZE_FACTOR;
        pirates->pirates = realloc(pirates->pirates,pirates->max_size*sizeof(pirate*));
        fprintf(stderr,"Expand to %ld\n", pirates->max_size);
    }
}

// CONSTRACT PIRATE LIST
void contractIfNecessary (pirate_list *pirates) {
    if(pirates->cur_size*RESIZE_FACTOR < pirates->max_size && pirates->max_size != INITIAL_CAPACITY) {
        pirates->max_size /= RESIZE_FACTOR;
        pirates->pirates = realloc(pirates->pirates, pirates->max_size*sizeof(pirate*));
        fprintf(stderr,"Contract to %ld\n", pirates->max_size);
    }
}

// create list and allocate memory inside
pirate_list *list_create() {
    pirate_list *ret = malloc(sizeof(pirate_list));
    ret->cur_size = 0;
    ret->max_size = INITIAL_CAPACITY;
    ret->pirates = malloc(ret->max_size*sizeof(pirate*));
    
    return ret;
}

// get index of the element of the list
size_t list_index_of(pirate_list *pirates, pirate *p) {
    for (int i=0; i < pirates->cur_size; i++){
        if (strcmp(pirate_get_name(pirates->pirates[i]), pirate_get_name(p)) == 0)
            return i;
    }
    return pirates->max_size+1;
}

// insert element in the list
pirate *list_insert(pirate_list *pirates, pirate *p, size_t idx) {
    if (list_index_of(pirates, p) > pirates->cur_size) {
        expandIfNecessary(pirates); // reallocate memmory
        for (int i = pirates->cur_size; i > idx; i--)
            pirates->pirates[i] = pirates->pirates[i-1];
        
        pirates->cur_size++;
        pirates->pirates[idx] = p; // insert the element
    } else {
        return p;
    }
    
    return NULL;
}

// get pirate by id
pirate *list_access(pirate_list *pirates, size_t idx) {
    if (idx >= list_length(pirates))
        return NULL;
    return pirates->pirates[idx];
}

// get current length of the pirates list
size_t list_length(pirate_list *pirates) {
    return pirates->cur_size;
}

// remove pirate by id
pirate *list_remove(pirate_list *pirates, pirate *prt) {
    size_t length = pirates->cur_size;
    size_t idx = list_index_of(pirates, prt);
    pirate *removed = pirates->pirates[idx];
    int i;

    for (i = idx; i < length; i++) {
        pirates->pirates[i] = pirates->pirates[i+1];
    }

    pirates->cur_size--;
    contractIfNecessary(pirates);
    return removed;
}

// sort function
void list_sort(pirate_list *pirates) {
    int n = list_length(pirates);
    
    // selection sort
    for(int i = 0; i < n - 1; ++i) {
        int pos = i;

        // IF all pirates have NULL name, no need to sort further
        while (pos < n && pirate_get_name(pirates->pirates[pos]) == NULL)
            pos++;

        // Then Break
        if (pos >= n)
            break;

        for (int j = pos + 1; j < n; ++j) {
            if (pirate_get_name(pirates->pirates[j]) == NULL) {
                // Move pirate with NULL name to the front
                pirate *temp = pirates->pirates[pos];
                pirates->pirates[pos] = pirates->pirates[j];
                pirates->pirates[j] = temp;
                pos++;
            } else if (strcmp(pirate_get_name(pirates->pirates[pos]), pirate_get_name(pirates->pirates[j])) > 0) {
                // ELSE: Compare name using strcmp
                pirate *temp = pirates->pirates[pos];
                pirates->pirates[pos] = pirates->pirates[j];
                pirates->pirates[j] = temp;
            }
        }
    }
}

// free memory
pirate *prt; int i = 0;
void list_destroy(pirate_list *pirates) {

    while (i < list_length(pirates)) {
        pirate *prt = list_access(pirates, i);
        pirate *to_free = list_remove(pirates, prt);
        pirate_destroy(to_free); // delete after printing
    }

    i++;
    free(pirates->pirates);
    free(pirates); // free list
}
