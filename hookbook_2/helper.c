#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pirate_list.h"

/////////////////////////////////////////////////////
// PRINT DATA ///////////////////////////////////////
/////////////////////////////////////////////////////

// print stars function
void printStars (size_t c) {
    while (c != 0) {
        printf("*");
        c--;
    }
}

// print ordered skills function with asterix
void printSkills(skills access) {
    int i = 0;
    printf("    Skills: ");

    if (access.cur_size == 0)
        printf("(None)");

    while (i < access.cur_size) {
        int count = 1;

        while (i + count < access.cur_size && strcmp(access.skills[i], access.skills[i + count]) == 0) {
            count++;
        }

        char *skill = access.skills[i];
        printf("%s ", skill);

        printStars(count);

        i += count;
        if (i < access.cur_size) {
            printf("\n            ");
        }
    }

    printf("\n");
}

// selection sort skills function
void sortSkills (skills *access) {
    size_t n = access->cur_size; // size
    
    // selection sort algorithm
    for(int i = 0; i < n - 1; ++i)
        for(int j = i + 1; j < n; ++j)
            if(strcmp(access->skills[i], access->skills[j]) > 0) {
                char *temp = access->skills[i];
                access->skills[i] = access->skills[j];
                access->skills[j] = temp;
            }
}

// prinf all data function
void print_data (pirate_list *pts) {
    size_t i = 0;

    while (i < list_length(pts)) {

        pirate *prt = list_access(pts, i);
        pirate *cpt = pirate_get_captain(prt);

        printf("Pirate: %s\n", pirate_get_name(prt));
        printf("    Title: %s\n", pirate_get_title(prt));

        printf("    Captain: %s\n", pirate_get_name(cpt));
        if (cpt != NULL) {
            printf("        Captain's Title: %s\n", pirate_get_title(cpt));
            printf("        Captain's Favorite Port of Call: %s\n", pirate_get_port(cpt));
        }

        printf("    Vessel: %s\n", pirate_get_vessel(prt));
        printf("    Treasure: %ld\n", prt->treasure);
        printf("    Favorite Port of Call: %s\n", pirate_get_port(prt));
        
        sortSkills(&prt->skills_list);
        printSkills(prt->skills_list);

        i++;
        printf("\n");
    }
}

/////////////////////////////////////////////////////
// END PRINT DATA ///////////////////////////////////
/////////////////////////////////////////////////////
