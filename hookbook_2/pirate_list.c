// LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// HEADERS
#include "pirate_list.h"

// GLOBAL
#define INITIAL_CAPACITY 25
#define RESIZE_FACTOR 2

// PIRATE IMPLEMENTATION
struct implementation {
    size_t max_size;
    size_t cur_size;
    pirate **pirates;
};

/////////////////////////////////////////////////////
// PIRATE LIST FUNCTIONS ////////////////////////////
/////////////////////////////////////////////////////

// EXPAND PIRATE LIST
void expandIfNecessary (pirate_list *pirates) {
    if (pirates->cur_size == pirates->max_size) {
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

// CREATE PIRATE LIST
pirate_list *list_create() {
    pirate_list *list = malloc(sizeof(pirate_list));
    list->cur_size = 0;
    list->max_size = INITIAL_CAPACITY;
    list->pirates = malloc(list->max_size*sizeof(pirate*));
    
    // return
    return list;
}

// GET INDEX OF PIRATE IN THE LIST
size_t list_index_of(pirate_list *pirates, pirate *p) {
    for (int i=0; i < pirates->cur_size; i++)
        if (strcmp(pirate_get_name(pirates->pirates[i]), pirate_get_name(p)) == 0)
            return i;

    // return
    return pirates->max_size;
}

// INSERT PIRATE IN THE LIST
pirate *list_insert(pirate_list *pirates, pirate *p, size_t idx) {
    if (list_index_of(pirates, p) > pirates->cur_size) {
        expandIfNecessary(pirates); // reallocate memmory
        for (int i = pirates->cur_size; i > idx; i--)
            pirates->pirates[i] = pirates->pirates[i-1];
        
        pirates->cur_size++;
        pirates->pirates[idx] = p; // insert the element

        // return
        return NULL;
    } else {    
        // return
        return p;
    }
}

// GET PIRATE BY INDEX
pirate *list_access(pirate_list *pirates, size_t idx) {
    if (idx >= pirates->cur_size)
        return NULL;
    else
        return pirates->pirates[idx];
}

// REMOVE PIRATE IN THE LIST
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

    // return
    return removed;
}

// LENGTH OF PIRATE LIST
size_t list_length(pirate_list *pirates) {
    return pirates->cur_size;
}

/////////////////////////////////////////////////////
// SORTING //////////////////////////////////////////
/////////////////////////////////////////////////////

// SORT BY -n
void list_sort(pirate_list *pirates) {
    int n = list_length(pirates);
    
    // selection sort
    for(int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j) {
            
            if(strcmp(pirate_get_name(pirates->pirates[i]), pirate_get_name(pirates->pirates[j])) > 0) {
                pirate *temp = pirates->pirates[i];
                pirates->pirates[i] = pirates->pirates[j];
                pirates->pirates[j] = temp;
            }
        }
    }
}

// SORT BY -v
void list_sort_v(pirate_list *pirates) {
    int n = list_length(pirates);
    
    // selection sort
    for(int i = 0; i < n - 1; ++i) {
        int pos = i;

        // IF all pirates have NULL vessels, no need to sort further
        while (pos < n && pirates->pirates[pos]->vessel == NULL)
            pos++;

        // Then Break
        if (pos >= n)
            break;

        for (int j = pos + 1; j < n; ++j) {
            if (pirates->pirates[j]->vessel == NULL) {
                // Move pirate with NULL vessel to the front
                pirate *temp = pirates->pirates[pos];
                pirates->pirates[pos] = pirates->pirates[j];
                pirates->pirates[j] = temp;
                pos++;
            } else if (strcmp(pirates->pirates[pos]->vessel, pirates->pirates[j]->vessel) > 0) {
                // ELSE: Compare vessels using strcmp
                pirate *temp = pirates->pirates[pos];
                pirates->pirates[pos] = pirates->pirates[j];
                pirates->pirates[j] = temp;
            }
        }
    }
}

// SORT BY -t
void list_sort_t(pirate_list *pirates) {
    int n = list_length(pirates);

    // selection sort
    for(int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j) {
            // if (pirates->pirates[i]->vessel == NULL || pirates->pirates[j]->vessel == NULL)
            //     return;

            if(pirates->pirates[i]->treasure < pirates->pirates[j]->treasure) {
                pirate *temp = pirates->pirates[i];
                pirates->pirates[i] = pirates->pirates[j];
                pirates->pirates[j] = temp;
            }
        }
    }
}

/////////////////////////////////////////////////////
// END SORTING //////////////////////////////////////
/////////////////////////////////////////////////////

// DESTROY PIRATE LIST
pirate *prt; int i = 0;
void list_destroy(pirate_list *pirates) {  
    while (i < list_length(pirates)) {
        pirate *prt = list_access(pirates, i);
        pirate *to_free = list_remove(pirates, prt);
        pirate_destroy(to_free); // delete after printing
    }


    // free
    free(pirates->pirates);
    free(pirates); // free list

    i++;
}
