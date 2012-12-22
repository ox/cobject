#ifndef _COBJECT_H
#define _COBJECT_H

#include <stdlib.h>
#include <string.h>

struct COClass {
  void (*destructor)(void*);
  int size;
};

struct COGuts {
  int reference_count;
  int is_on_heap;
  struct COClass * class;
};

void * COCreate(struct COClass * class);
void COInit(void * obj, struct COClass * class);

void * CORetain(void * obj);
void _CODestroyAsClass(void * obj, struct COClass * class);
void CORelease(void * obj);

void COAutoCleanup(void * obj);
#endif
