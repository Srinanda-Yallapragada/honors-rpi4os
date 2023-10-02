#include "io.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// This function returns the number of characters in a string including the '\0'
// or null terminator char
int lenstr(char *str) {
  int len = 0;
  while (*str != '\0') {
    len++;
    str++;
  }
  return len;
}

// also called cmpstr to avoid annoying warnings
//  this function should return 1 if equal and -1 if unequal
// int cmpstr(char str1[], char str2[]) {
//   int ret_val = 1; // assume equal till proven otherwise
//   int len_str_1 = lenstr(str1);
//   int len_str_2 = lenstr(str2);

//   if (len_str_1 != len_str_2) {
//     return -1;
//   }

//   for (int i = 0; i < len_str_1; i++) {
//     if (str1[i] != str2[i]) {
//       ret_val = -1;
//     }
//   }

//   return ret_val;
// }

// Taken from this stack overflow post
// https://stackoverflow.com/questions/34873209/implementation-of-strcmp
int cmpstr(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
