#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

    /* TODO: Add more members if necessary */
  char expr[1024];
  word_t value;


} WP;

WP* new_WP();
void free_WP(WP *node);
int query_WP();

#endif
