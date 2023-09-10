// LIBS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HEADERS
#include "pirate.h"

// GLOBAL
#define INITIAL_CAPACITY 25 // it's better be 2 to some power
#define RESIZE_FACTOR 2
#define MAX_STR_SIZE 65

/////////////////////////////////////////////////////
// PIRATE FUNCTIONS /////////////////////////////////
/////////////////////////////////////////////////////

// CREATE PIRATE
pirate* pirate_create (const char* name) {
    pirate* p = malloc(sizeof(pirate));
    p->name = calloc((strlen(name) + 1), sizeof(char));
    p->captain = NULL;
    p->title = NULL;
    p->port = NULL;
    p->treasure = 0;
    strcpy(p->name, name);

    p->skills_list.cur_size = 0;
    p->skills_list.max_size = INITIAL_CAPACITY;
    p->skills_list.skills = malloc(sizeof(char)*MAX_STR_SIZE*INITIAL_CAPACITY);
    
    // return
    return p;
}


// PIRATE GETTERS
char *pirate_get_name(pirate* prt) {
    if (prt != 0) {
        if (prt->name != 0)
            return prt->name;
        else
            return "(None)";
    } else {
        return "(None)";
    }
}

pirate *pirate_get_captain(pirate* prt) {
    return prt->captain;
}

char *pirate_get_vessel(pirate* prt) {
    if (prt != 0) {
        if (prt->vessel != 0)
            return prt->vessel;
        else
            return "(None)";
    } else {
        return "(None)";
    }
}

char *pirate_get_title(pirate* prt) {
    if (prt != 0) {
        if (prt->title != 0)
            return prt->title;
        else
            return "(None)";
    } else {
        return "(None)";
    }
}

char *pirate_get_port(pirate* prt) {
    if (prt != 0) {
        if (prt->port != 0)
            return prt->port;
        else
            return "(None)";
    } else {
        return "(None)";
    }
}


// DESTROY PIRATE
void pirate_destroy(pirate *prt) {

    for (int j = 0; j < prt->skills_list.cur_size; j++){
        free(prt->skills_list.skills[j]);
    }
    free(prt->skills_list.skills);

    free(prt->port);
    free(prt->title);
    free(prt->vessel);
    free(prt->name);

    free(prt);
}

/////////////////////////////////////////////////////
// SKILLS FUNCTIONS /////////////////////////////////
/////////////////////////////////////////////////////

// EXPAND PIRATE SKILLS MEMORY
void expandIfNecessarySkills(skills *s) {
    if (s->cur_size == s->max_size) {
        s->max_size *= RESIZE_FACTOR;
        s->skills = realloc(s->skills, sizeof(char*)*s->max_size);
    }
}

// INSERT SKILL IN THE LIST OF PIRATE->SKILLS
void skill_insert(skills *s, char *skill) {
    expandIfNecessarySkills(s);

    s->skills[s->cur_size] = malloc(sizeof(char)*(strlen(skill)+1));
    strcpy(s->skills[s->cur_size], skill);
    s->cur_size++;
}


