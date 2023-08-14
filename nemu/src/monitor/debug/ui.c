#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <memory/vaddr.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
  int step = args ? atoi(args) : 1;
  step = step <= 0 ? 1 : step;
  cpu_exec(step);
  return 0;
}

static int cmd_info(char* args){
  if(args==NULL)
    return 0;
  if(args[0]=='r')
    isa_reg_display();  
  return 0;
}

static int cmd_x(char* args){
  int len;
  vaddr_t addr;
  sscanf(args, "%d %x",&len, &addr);
  for (int i=0; i<len;i++)
    printf("%02x ",vaddr_read(addr+i, 1));
  printf("\n");
  return 0;
}

static int cmd_p(char* args){
  bool success;
  word_t ret=expr(args,&success);
  if(success==true)
    printf("the value is %u\n", ret);
  else
    printf("invalid expression!");
  return 0;
}

static int cmd_ptest(char*args){
  FILE *fp = fopen("tools/gen-expr/input", "r");
  assert(fp != NULL);
  word_t res;
  char testexpr[1000]="";
  int cnt = 0;
  while (fscanf(fp,"%u %s", &res, testexpr) == 2)
  {
    bool success;
    word_t res_nemu=expr(testexpr,&success);
    Log("Test %03d: %s, result=%u, nemu=%u, expr=%s", cnt++, res == res_nemu ? "success" : "   fail", res, res_nemu, testexpr);
    // cmd_p(exprr);
    // printf("number %d test, res = %u\n",i+1,res);
    // printf("####################\n");
  }
  fclose(fp);
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "execute steps cmd", cmd_si},
    {"info", "prinf status of cpu", cmd_info},
    {"x","scan the memory", cmd_x},
    {"p","get value of an expression",cmd_p},
    {"ptest","test the ceshi examples",cmd_ptest},
/* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
