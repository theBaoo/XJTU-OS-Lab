#include "user.h"

int strlen  (const char *s) {
    int i = 0;
    while (s[i++]);
}

int strcmp  (const char *s, const char *d) {
    int i = 0;
    while (s[i] && d[i] && s[i] == d[i]) i++;
    return s[i] - d[i];
}


// __attribute__((noinline)) 
int uprintf(const char *fmt) {
    return uwrite(CONSOLE, fmt, 0);
}

// __attribute__((noinline)) 
int uprintfi(const char *fmt, int num) {
    return uwrite(CONSOLE, fmt, num);
}
// __attribute__((noinline)) 
int gets(char *buf, int len) {
    (void)buf;
    (void)len;
    return uread(CONSOLE, buf, len);
    
    // char c;
    // int i, tmp;
    // for (i = 0; i < len;) {
    //     tmp = uread(CONSOLE, &c, 1);
    //     if (tmp < 1) break;
    //     buf[i++] = c;
    //     if (c == '\r' || c == '\n') break;
    // }
    // buf[i] = '\0';
    // return i;
}