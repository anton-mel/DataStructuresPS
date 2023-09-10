#ifndef __PIRATE_H__
#define __PIRATE_H__

// SKILLS STRUCT
typedef struct skills_struct {
    size_t cur_size;
    size_t max_size;
    char **skills;
} skills;

// PIRATE STRCUT
struct pirate_struct {
    // name of pirate
    char *name;

    // and some other imformation
    char *title;
    struct pirate_struct *captain;
    char *vessel;
    char *port;
    size_t treasure;
    
    struct skills_struct skills_list;
};

// TYPEDEF PIRATE & SKILLS
typedef struct pirate_struct pirate;
typedef struct implementation_skills skills_list;


// PIRATE STRUCT FUNCTIONS
void store_pirate_field (pirate *pirate, char *label, char *value);
void pirate_destroy(pirate *prt);
pirate* pirate_create(const char* name);


// PIRATE STRUCT GETTERS
char *pirate_get_name(pirate* prt);
char *pirate_get_title(pirate* prt);
char *pirate_get_vessel(pirate* prt);
char *pirate_get_port(pirate* prt);
pirate *pirate_get_captain(pirate* prt);

// SKILLS STRUCT FUNCTIONS
void expandIfNecessarySkills(skills *s);
size_t skills_index_of(pirate *p, char *skill);
void skill_insert(skills *s, char *skill);

#endif