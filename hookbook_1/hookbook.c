#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pirate_list.h"

#define MAX_STR_SIZE (85)

int main (int argc, char **argv) {

    if (argc < 2)
        return 1;
    
    FILE *pirates_file = fopen(argv[1], "r");
    if (pirates_file == NULL)
        return 1;

    // define new struct with mem info and list of pirates
    pirate_list *pirates_mem = list_create(); // create pirate by list_create() function
    char name[MAX_STR_SIZE];

    // iterate new pirates' names where firsly allocate new memory
    while (fgets(name, MAX_STR_SIZE, pirates_file) != NULL) {
        if (name[strlen(name)-1] == '\n') 
            name[strlen(name)-1] = '\0';
        
        if (name[strlen(name)-1] != '\0') // if file ends with no \n
            name[strlen(name)] = '\0';

        pirate* prt = pirate_create(name);

        // add new pirate
        pirate *temp = list_insert(pirates_mem, prt, list_length(pirates_mem));
        if (temp != NULL) {
            pirate_destroy(temp);
        }
    }
    

    list_sort(pirates_mem); // buble sort

    //output
    pirate *temp;
    for (int i = 0; i < list_length(pirates_mem); i++) {
        temp = list_access(pirates_mem, i);
        printf("%s\n", pirate_get_name(temp));
    }

    list_destroy(pirates_mem);
    fclose(pirates_file); // close the file

    return 0;
}
