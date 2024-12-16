#include "user.h"
#include "spinlock.h"

struct sh_cmd cmds[] = {
    {"h", "help", help},
    {"q", "quit", quit},
    {"t", "seconds since booting", time},
};

void test() {
    uprintf("test: call ups()\n");
    ups();
    uprintf("I am a child of sh to test fork.\n");
    uexit(0);
}

struct spinlock lk;
void test1() {
    acquire(&lk);
    uprintf("test1\n");
    release(&lk);
    uexit(0);
}

void test2() {
    acquire(&lk);
    uprintf("test2\n");
    release(&lk);
    uexit(0);
}

int sh() {
    char buf[128];
    // initlock(&lk, "sh");
    // if (ufork() == 0) {
    //     uexec("test1", (uint64)test1);
    //     uprintf("exec failed\n");
    //     while (1);
    // } 
    // if (ufork() == 0) {
    //     uexec("test2", (uint64)test2);
    //     uprintf("exec failed\n");
    //     while (1);
    // }
    // uwait(0);
    while (1) {
        uprintf("🥸  ");
        int num = gets(buf, MAX_CMD_LEN);
        parse_cmd(buf, num);

        // if (ufork() == 0) {
        //     uexec("test", (uint64)test);
        //     uprintf("exec failed\n");
        //     while (1);
        // } else {
        //     uwait(0);
        // }

    }
}

int parse_cmd(char *buf, int num) {
    if (num == 0) return 0;
    struct sh_cmd *cmd = find_cmd(buf);
    if (cmd) cmd->cmd_handler();
    else if (buf[0] >= '0' && buf[0] <= '9')
        uprintfi("The result is %d.\n", calculate(buf));
    else if (strcmp(buf, "gn") == 0)
        guessnum();
    else if (strcmp(buf, "ps") == 0)
        ups();
    else uprintfi("You typed %d words.\n", num);
    
}

struct sh_cmd *find_cmd(char *buf) {
    int size = sizeof(cmds) / sizeof(cmds[0]);
    for (int i = 0; i < size; i++) {
        if (strcmp(buf, cmds[i].cmd) == 0) {
            return &cmds[i];
        }
    }
    return 0;
}

int help() {
    uprintf("Help: \n");
    for (int i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        uprintf(cmds[i].cmd);
        uprintf(" : ");
        uprintf(cmds[i].desc);
        uprintf("\n");
    }
}

int quit() {
    uprintf("Bye!\n");
    // while (1);
    uexit(0);
}

int time() {
    uprintfi("It has been %d seconds since booting\n",
        timepoint / 10);
}

// -------------------
int isdigit(char c) {
    return c >= '0' && c <= '9';
}

int parse_number(const char **str) {
    int num = 0;
    while (isdigit(**str)) {
        num = num * 10 + (**str - '0');
        (*str)++;
    }
    return num;
}

int parse_term(const char **str) {
    int num = parse_number(str);
    while (**str == '*' || **str == '/') {
        char op = **str;
        (*str)++;
        int next_num = parse_number(str);
        if (op == '*') {
            num *= next_num;
        } else if (op == '/') {
            num /= next_num;
        }
    }
    return num;
}

int parse_expression(const char **str) {
    int num = parse_term(str);
    while (**str == '+' || **str == '-') {
        char op = **str;
        (*str)++;
        int next_num = parse_term(str);
        if (op == '+') {
            num += next_num;
        } else if (op == '-') {
            num -= next_num;
        }
    }
    return num;
}

int calculate(char *buf) {
    return parse_expression(&buf);
}

int guessnum() {
    int num = (timepoint * 12 + 4) % 100;
    int guess = 0;
    char buffer[128];
    uprintf("\nGuess a number between 1 and 100: ");

    while (1) {
        gets(buffer, 128);
        if (buffer[0] == 'q') break;
        guess = parse_number(&buffer);
        if (guess == num) {
            uprintf("Congratulations! You got it!\n");
            break;
        } else if (guess < num) {
            uprintf("Too small.\n");
        } else {
            uprintf("Too big.\n");
        }
    }
}