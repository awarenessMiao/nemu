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
void delete_WP(int NO);
int query_WP(int nemu_state);
void print_WP();

#endif
