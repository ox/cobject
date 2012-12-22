#include <stdio.h>
#include <assert.h>

#include "cobject.h"

/* the basic object */
struct Client {
  struct COGuts guts; /* internals for handling retain/release */

  int id;
  int credit;
};

/* the free method that should handle destruction of malloc'd data */
void client_destroy(void * a) {
  printf("destroying client insides, but none exist, so I'm gonna pass\n");
}

/* class description of the object, used for allocating and free'ing */
struct COClass ClientType = {
  .destructor = &client_destroy,
  .size = sizeof(struct Client)
};

int main(int argc, char ** argv) {
  struct Client * cli = COCreate(&ClientType);
  cli->id = 1;
  cli->credit = 10;

  assert(cli->id == 1);
  assert(cli->credit == 10);

  CORetain(cli);
  CORelease(cli);
  CORelease(cli);

  return 0;
}
