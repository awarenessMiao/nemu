#include <isa.h>
#include <memory/vaddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM , TK_HEX ,
  TK_REG , TK_AND , TK_NEQ ,TK_DEREF

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
  {"0x[0-9a-f]+",TK_HEX},
  {"[0-9]+",TK_NUM},    // number
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"\\$[a-z]+",TK_REG},
  {"&&",TK_AND},
  {"!=",TK_NEQ},

  
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

#define NUM_TOKENS 1024

static Token tokens[NUM_TOKENS] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

// only includes +-*/ == != && 
// not includes $ *(refers to引用)
int get_priority(int op_type) {
  switch (op_type) {
    case TK_DEREF: return 0;
    case '*':case '/': return 1;
    case '+':case '-': return 2;
    case TK_NEQ:case TK_EQ:return 3;
    case TK_AND:return 4;

    default: return -1; 
  }
}

// 词法分析
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

        if (nr_token >= NUM_TOKENS)
          return false;

        switch (rules[i].token_type) {
          case TK_NUM: case TK_HEX: case TK_REG:         
            if(substr_len>31)
              return false;
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len] = '\0';

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
  for(int i = 0;i< nr_token; i++){
    if(tokens[i].type == '*'&&(i == 0||get_priority(tokens[i-1].type)>=0
                              ||tokens[i-1].type == '('))
      tokens[i].type = TK_DEREF;
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
  int ops[NUM_TOKENS];
  int nr_ops = 0;
  for(int i = p ;i <= q ;i ++){
    if(tokens[i].type=='(')
    { // 跨越括号
      int j=i;
      while(check_parentheses(i,j)!=true)
        j++;
      i=j;
    }
    else if(get_priority(tokens[i].type)>=0) 
    // use priority func to match operator, >=0 is an op
    {
      // store the position of the operator in token
      ops[nr_ops++]=i;
    }
  }
  int op = 0;
  for(int i = 0;i<nr_ops;i++){
    if(get_priority(tokens[ops[i]].type)>get_priority(tokens[op].type))
      op=ops[i];
    if(get_priority(tokens[ops[i]].type) == get_priority(tokens[op].type)
    &&get_priority(tokens[op].type) >0)
      op=ops[i];
    // printf("%c ",tokens[ops[i]].type);
  }
  // printf("#\n");
  Log("find main op %c", tokens[op].type);
  return op;
}

// 语法分析
int eval(int p,int q, bool *success){
  *success = true;
  if (p > q) {
    *success = false;
    return 0;
  }
  if(p == q){
    Log("find a number %s", tokens[p].str);
    int res;
    switch(tokens[p].type){
      case TK_NUM: sscanf(tokens[p].str,"%d",&res); break;
      case TK_HEX: sscanf(tokens[p].str,"%x",&res); break;
      case TK_REG: 
        res = isa_reg_str2val(tokens[p].str+1,success);
        if(*success==false)
          return 0;
        break;
      default:
        *success = false;
        return 0;
    }
    return res;
  }
  else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1,success);
  }
  // else if(tokens[p].type == TK_DEREF){
  //   vaddr_t addr = eval(p+1,q,success);
  //   return vaddr_read(addr,4);
  // }
  else{
    int op = find_main_op(p,q);
    int op_type = tokens[op].type;
    if(op_type == TK_DEREF){ 
      vaddr_t addr = eval(op+1,q,success);
      return vaddr_read(addr,4);
    }
    bool success1,success2;
    int val1 = eval(p,op-1,&success1);
    int val2 = eval(op+1,q,&success2);
    if (success1 == false || success2 == false) {
      *success = false;
      return 0;
    }
    switch(op_type){
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': if (val2 != 0) return val1 / val2;
      case TK_AND: return val1&&val2;
      case TK_EQ: return val1==val2;
      case TK_NEQ: return val1!=val2;
      default:
        *success = false;
        return 0;
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */


  word_t evaluation=eval(0,nr_token-1,success);
  return evaluation;
}
