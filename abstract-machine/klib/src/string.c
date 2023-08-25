#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *p = s;
  int len = 0;
  while (*p != '\0'){
    p++;
    len++;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  return memmove(dst, src, strlen(src)+1);
}

char* strncpy(char* dst, const char* src, size_t n) {
  int len = strlen(src) + 1;
  return memmove(dst, src, n < len ? n : len);
}

char* strcat(char* dst, const char* src) {
  strcpy(dst + strlen(dst), src);
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  int len1 = strlen(s1), len2 = strlen(s2);
  // int min_len = strlen(s1) > strlen(s2) ? strlen(s2) : strlen(s1);
  return strncmp(s1, s2, len1 < len2 ? len1 : len2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return memcmp(s1, s2, n);
}

void* memset(void* v,int c,size_t n) {
  for (int i = 0; i < n;i++){
    ((unsigned char *)v)[i] = c;
  }
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  if(src > dst){
    for (int i = 0; i < n; i++)
      ((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
  }
  if(src < dst){
    for (int i = n-1; i >=0 ; i--)
      ((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
  }
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  for (int i = 0; i < n; i++){
    ((unsigned char *)out)[i] = ((const unsigned char *)in)[i];
  }
    return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  putch('a');
  for (int i = 0; i < n; i++){
    if (((const unsigned char *)s1)[i] < ((const unsigned char *)s2)[i]){
      putch('<');
      return -1;
    }
    if (((const unsigned char *)s1)[i] > ((const unsigned char *)s2)[i]){
      putch('>');
      return 1;
    }
  }
  return 0;
}

#endif
