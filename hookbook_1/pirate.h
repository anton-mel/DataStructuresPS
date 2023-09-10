#ifndef __PIRATE_H__
#define __PIRATE_H__

// Type of a pirate.
typedef struct pirate_struct pirate;

// free memmory for signle pirate
void pirate_destroy(pirate *prt);

// create pirate by a name value
pirate* pirate_create(const char* name);

// Get name of the pirate by passin its struct
char* pirate_get_name(pirate* prt);

#endif