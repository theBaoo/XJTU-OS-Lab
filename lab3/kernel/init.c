#include "user.h"

int init() {
    int pid, ppid;

startsh:
    uprintf("\nsh is booting!\n\n");
    if ((pid = ufork()) == 0) {
        uexec("sh", (uint64)sh);
    }

    ppid = uwait(0);
    if (ppid == pid) goto startsh;

    // never reach
    uexit(0);
}