// LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// HEADERS
#include "pirate_list.h"
#include "helper.h"

// GLOBAL
#define MAX_STR_SIZE (65)

// STATE MACHINE DEFINITION
typedef enum {
    PROFILE_BEGIN_STATE, PROFILE_CONTENT_STATE
} input_state;

int main (int argc, char **argv) {
    // HANDLE ARGUMENTS ERRORs
    if (argc < 4) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    // OPEN & SAVE
    FILE *pirates_profiles = fopen(argv[1], "r"); // pirates profiles
    FILE *pirates_pairs = fopen(argv[2], "r"); // pirate and captain pairs
    char *sort_type = argv[3]; // pirate and captain pairs

    // HANDLE FILE ERRORs
    if (pirates_profiles == NULL) {
        fprintf(stderr, "Invalid filename: %s\n", argv[1]);
        return 1;
    }

    if (pirates_pairs == NULL) {
        fprintf(stderr, "Invalid filename: %s\n", argv[2]);
        return 1;
    }

    /////////////////////////////////////////////////////
    // READ FILE 1 //////////////////////////////////////
    /////////////////////////////////////////////////////

    // define
    pirate *current_pirate;
    pirate_list *pirates_mem = list_create(); // create pirate by list_create() function

    input_state current_state = PROFILE_BEGIN_STATE;
    char lable[65]; char value[65]; // values lable and value
    char pairlable[65]; char captain[65]; // values lable and value
    int line = fscanf(pirates_profiles, "%[^:]: %[^\n]\n", lable, value);

    // STATE MACHINE
    while (line > 0) {
        // DELETE ALL \n AND REPLACE WITH \0
        if (value[strlen(value)-1] == '\n') 
            value[strlen(value)-1] = '\0';

        // SWITCH CASES
        switch (current_state) {
            case PROFILE_BEGIN_STATE:
                
                // finds name => create new pirate
                current_pirate = pirate_create(value); // create new pirate
                current_state = PROFILE_CONTENT_STATE;

            case PROFILE_CONTENT_STATE:

                // pull up the information in the pirate
                memset(lable, 0, sizeof(lable)); // clear lable
                line = fscanf(pirates_profiles, "%[^:]:%[^\n]\n", lable, value);

                if (strcmp(lable, "name") == 0) {
                    // if name, change state back
                    list_insert(pirates_mem, current_pirate, list_length(pirates_mem));
                    current_state = PROFILE_BEGIN_STATE;
                } else if (strcmp(lable, "title") == 0) {
                    current_pirate->title = calloc((strlen(value) + 1), sizeof(char));
                    strcpy(current_pirate->title, value);
                } else if (strcmp(lable, "vessel") == 0) {
                    current_pirate->vessel = calloc((strlen(value) + 1), sizeof(char));
                    strcpy(current_pirate->vessel, value);
                } else if (strcmp(lable, "port") == 0) {
                    current_pirate->port = calloc((strlen(value) + 1), sizeof(char));
                    strcpy(current_pirate->port, value);
                } else if (strcmp(lable, "treasure") == 0) {
                    current_pirate->treasure = atoi(value);
                } else if (strcmp(lable, "skill") == 0) {
                    skill_insert(&current_pirate->skills_list, value);
                } else {
                    break;
                }

            default:
                break;
        }
        
        // INSERT LAST: the last pirate will be created but not inserted
        list_insert(pirates_mem, current_pirate, list_length(pirates_mem));
    }

    /////////////////////////////////////////////////////
    // READ FILE 2 //////////////////////////////////////
    /////////////////////////////////////////////////////

    pirate *underPiratePair = NULL;
    pirate *abovePiratePair = NULL;

    // scan second file
    while (fscanf(pirates_pairs, "%[^/]/ %[^\n]\n", pairlable, captain) > 0) {
        // DELETE ALL \n AND REPLACE WITH \0
        if (captain[strlen(captain)-1] == '\n') 
            captain[strlen(captain)-1] = '\0';

        // FIND CAPTAIN & PIRATE
        for (int i = 0; i < list_length(pirates_mem); i++) {
            // current pirate
            pirate *loopPirate = list_access(pirates_mem, i);

            // pirates with the same name
            if (strcmp(pirate_get_name(loopPirate), pairlable) == 0) {
                underPiratePair = loopPirate;
            }

            // captains with the same name
            if (strcmp(pirate_get_name(loopPirate), captain) == 0) {
                abovePiratePair = loopPirate;
            }
        }

        // ADD CAPTAIN TO THE PIRATE
        underPiratePair->captain = abovePiratePair;
    }

    // SORTING ALGORITHMS
    if (strcmp(sort_type, "-n") == 0) {
        list_sort(pirates_mem); // selection sort
    } else if (strcmp(sort_type, "-v") == 0) {
        list_sort_v(pirates_mem); // selection sort
    } else if (strcmp(sort_type, "-t") == 0) { 
        list_sort_t(pirates_mem); // selection sort
    } else {
        // HANDLE ERROR
        fprintf(stderr, "Invalid argument: %s\n", sort_type);
        return 1;
    }

    // OUTPUT
    print_data(pirates_mem);

    // FREEING
    list_destroy(pirates_mem); // free memmory

    // CLODE FILES
    fclose(pirates_profiles); // close the file
    fclose(pirates_pairs); // close the file

    // RETURN
    return 0;
}
