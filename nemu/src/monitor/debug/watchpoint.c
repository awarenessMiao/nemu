#include "watchpoint.h"
#include "expr.h"

#include <monitor/monitor.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

void insert_WP(WP** list, WP* node){
  node->next = *list;
  *list = node;
}

void remove_WP(WP** list, WP* node){
  WP* p = *list;
  if(p == node) {
    *list = node->next;
    return;
  }
  Assert(p != NULL,"list is empty");
  while(p->next != node){
    p = p->next;
    Assert(p != NULL, "node not found in list");
  }
  p->next = node->next;

}

WP* new_WP(){
  WP *node = free_;
  remove_WP(&free_, node);
  insert_WP(&head, node);
  return node;
}

void free_WP(WP *node) {
  remove_WP(&head, node);
  insert_WP(&free_, node);
}

int query_WP(int nemu_state) {
  WP* p = head;
  if(p == NULL){
    // printf("no watchpoint\n");
    return nemu_state;
  }
  bool success;
  while(p != NULL){
    word_t cur_value = expr(p->expr, &success);
    // printf("No %d watchpoint is checked,pre-value is %x, current value is %x\n",
    //         p->NO,p->value,cur_value);
    if(cur_value != p->value){
      printf("No %d watchpoint is changed,pre-value is %x, current value is %x\n",
            p->NO,p->value,cur_value);
      p->value = cur_value;
      nemu_state = nemu_state == NEMU_RUNNING ? NEMU_STOP : nemu_state;
    }
    p = p->next;
  }
  return nemu_state;
}

void print_WP() {
  for(WP *p=head; p!=NULL; p=p->next)
    printf("No %x wp: value is %x, it watches %s",p->NO,p->value,p->expr);
}

void delete_WP(int NO) {
  for(WP *p=head; p!=NULL; p=p->next) {
    if (p->NO==NO)
      free_WP(p);
  }
}