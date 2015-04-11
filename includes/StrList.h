#ifndef STRLIST_H
#define STRLIST_H

typedef struct strlist {
  char * clients[3000];
  int size;
} * StrList;

#endif
