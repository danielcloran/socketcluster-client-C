#ifndef __LOGGER_H__
#define __LOGGER_H__

#define KCYN_L "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[0;33m"
#define RESET "\033[0m"

enum log_level
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

void scc_log(int level, char *string);
void set_scc_log_func(void (*)(int, char*));
void default_log_func(int level, char *string);

extern void (*scc_log_func)(int, char *);

#endif // __LOGGER_H__