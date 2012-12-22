#include "cobject.h"

void * COCreate(struct COClass * class) {
  struct COGuts * guts = malloc(class->size);
  COInit(guts, class);
  guts->is_on_heap = 1;
  return guts;
}

void COInit(void * obj, struct COClass * class) {
  struct COGuts * guts = obj;
  bzero(obj, class->size);

  guts->reference_count = 1;
  guts->class = class;
}

void * CORetain(void * obj) {
  struct COGuts * guts = obj;
  guts->reference_count++;
  return obj;
}

void _CODestroyAsClass(void * obj, struct COClass * class) {
  if (!class)
    return;

  if (class->destructor)
    class->destructor(obj);
}

void CORelease(void * obj) {
  struct COGuts * guts = obj;
  guts->reference_count--;

  if (guts->reference_count == 0) {
    _CODestroyAsClass(obj, guts->class);

    if (guts->is_on_heap)
      free(obj);
  }
}
