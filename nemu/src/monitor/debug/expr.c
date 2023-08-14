#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"[0-9]+",TK_NUM},    // number
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NUM:           
            if(substr_len>31)
              return false;
            strncpy(tokens[nr_token].str,substr_start,substr_len);
          default: 
            tokens[nr_token].type=rules[i].token_type;
            nr_token++;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p,int q){
  if(tokens[p].type!='('|| tokens[q].type!=')')
    return false;
  int stack = 0;
  for(int i=p+1;i<=q-1;i++)
  {
    if(tokens[i].type=='(')
      stack++;
    else if(tokens[i].type==')')
      stack--;
    if(stack<0)
      return false;
  }
  return stack == 0;
}

int find_main_op(int p,int q){
  int op = -1;
  int ops[32];
  int nr_ops = 0;
  for(int i=p;i<=q;i++){
    if(tokens[i].type=='(')
    {
      int j=i;
      while(check_parentheses(i,j)!=true)
        j++;
      i=j;
    }
    else if(tokens[i].type=='+'||tokens[i].type=='-'||
    tokens[i].type=='*'||tokens[i].type=='*')
    {
      ops[nr_ops++]=i;
    }
  }
  for(int i=0;i<nr_ops;i++){
    if(tokens[ops[i]].type=='+'||tokens[ops[i]].type=='-')
      op = ops[i];
  }
  return op=-1?ops[nr_ops-1]:op;
}

int eval(int p,int q){
  Assert(p <= q, "invalid expression:too");
  if(p == q){
    Assert(tokens[p].type == TK_NUM, "not a number");
    return atoi(tokens[p].str);
  }
  else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1);
  }
  else{
    int op = find_main_op(p,q);
    int op_type=tokens[op].type;
    int val1 = eval(p,op-1);
    int val2 = eval(op+1,q);
    switch(op_type){
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default:assert(0);
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  
  int p=0;int q=nr_token-1;
  int evaluation=eval(p,q);
  *success = true;
  return evaluation;

  return 0;
}
