#include "types.h"
#define MAX_CMD_LEN 128
#define CONSOLE 1

extern int timepoint;

struct sh_cmd {
    char *cmd;
    char *desc;
    int (*cmd_handler)();
};

extern struct sh_cmd cmds[];

int sh();

// shell-builtin
int parse_cmd(char *buf, int num);
struct sh_cmd *find_cmd(char *buf);
int help();
int quit();
int handler();
int time();
int calculate(char *buf);

int guessnum();

// ulib
int uprintf (const char *fmt);
int uprintfi(const char *fmt, int num);
int gets    (char *buf, int len);
int strlen  (const char *);
int strcmp  (const char *, const char *);

// syscall
int uwrite(int fd, char *buf, int len);
int uread (int fd, char *c, int len);
int ufork();
int uexec(char *, uint64);
int uexit(int status);
int uwait(int *status);
int usleep(int time);
int ups();