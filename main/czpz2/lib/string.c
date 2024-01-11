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
// Taken from this stack overflow post
// https://stackoverflow.com/questions/34873209/implementation-of-strcmp
int cmpstr(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}


//taken from  this stack overflow post
//https://stackoverflow.com/questions/9994742/want-to-convert-integer-to-string-without-itoa-function
//
void getDecStr (char* str, unsigned int len, unsigned int val)
{
  int i;

  for(i=1; i<=len; i++)
  {
    str[len-i] = (int) ((val % 10UL) + '0');
    val/=10;
  }

  str[i-1] = '\n';
}
