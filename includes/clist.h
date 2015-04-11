#ifndef CLIST_H
#define CLIST_H

typedef struct listnode {
  char code[6];
  struct listnode * next;
} CList;

#endif
