#ifndef MAIN_H
#define MAIN_H

typedef struct _xmlDocSet {
    xmlDocPtr doc;
    struct _xmlDocSet * next;
    xmlNodePtr cur;		
    int proctype;
    int processed;
    struct _variable *param;
} xmlDocSet;

typedef struct _variable {
    char * name;
    char * data;
    struct _variable *next;
} variable;

#endif /* ifndef MAIN_H */

