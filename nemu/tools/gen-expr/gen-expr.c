#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n)
{
  return rand()%n;
}

void gen_num(){
  sprintf(buf+strlen(buf),"%u",rand());
}
void gen(char ch){
  // printf("before gen(): %s\n", buf);
  sprintf(buf+strlen(buf),"%c",ch);
  // printf("after gen(): %s\n", buf);
}
void gen_rand_op(){
  switch(choose(7))
  {
    case 0:case 4:gen('+');break;
    case 1:case 5:gen('-');break;
    case 2:case 6:gen('*');break;
    case 3:gen('/');break;
  }
}

static inline void gen_rand_expr() {
  if(strlen(buf)>100) {
    gen('1');
    return;
  }
  switch(choose(3))
  {
    case 0:gen_rand_expr();gen_rand_op();gen_rand_expr();break;
    case 1:gen('(');gen_rand_expr();gen(')');break;
    default:gen_num();break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[0]='\0';
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    // printf("%s\n", buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
