#include "../include/logger.h"
#include <stdlib.h>
#include <stdio.h>

void (*scc_log_func)(int, char *) = &default_log_func;

void scc_log(int level, char *string)
{
    scc_log_func(level, string);
}

void default_log_func(int level, char *string)
{
    switch (level)
    {
    case DEBUG:
        printf(KCYN_L "scc - [DEBUG]%s" RESET, string);
        break;
    case INFO:
        printf("scc - [INFO] %s", string);
        break;
    case WARN:
        printf(KYEL "scc - [WARN] %s" RESET, string);
        break;
    case ERROR:
        printf(KRED "scc - [ERROR]%s" RESET, string);
        break;
    default:
        printf("scc - %s", string);
    }
}

void set_scc_log_func(void (*func)(int, char*))
{
    scc_log_func = func;
}

/* int main()
{
    scc_log(DEBUG, "Debug example.\n");
    scc_log(INFO, "Info example.\n");
    scc_log(WARN, "Warn example.\n");
    scc_log(ERROR, "Error example.\n");
    return EXIT_SUCCESS;
} */