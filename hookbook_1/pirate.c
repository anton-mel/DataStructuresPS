#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pirate.h"

// single pirate struct
struct pirate_struct {
    char *name; // name of pirate
    // and some other imformation if neces
};

// create pirate function
pirate* pirate_create (const char* name){
    pirate* p = malloc(sizeof(pirate));
    p->name = calloc((strlen(name) + 1), sizeof(char));
    strcpy(p->name, name);
    
    return p;
}

// get name of pirate function
char* pirate_get_name(pirate* prt){
    if (prt == NULL)
        return NULL;

    return prt->name;
}

// destroy allocated memory of pirate
void pirate_destroy(pirate *prt){
    free(prt->name);
    free(prt);
}