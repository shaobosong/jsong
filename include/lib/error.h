#ifndef lib_error_h
#define lib_error_h

#include <errno.h>

#define L_RED      "\e[1;31m"
#define L_GREEN    "\e[1;32m"
#define BOLD       "\e[1m"
#define NONE       "\e[0m"

#define THROW_EXCEPTION(msg) \
    err_sys(BOLD"%s:%d: "NONE"[----"L_RED"%s"NONE"----]: "msg, __FILE__, __LINE__, __func__)
#define THROW_WARNING(msg) \
    err_msg(BOLD"%s:%d: "NONE"[----"L_RED"%s"NONE"----]: "msg, __FILE__, __LINE__, __func__)
#define MY_ASSERT(expr) \
do \
{ \
    if (expr) \
        ; /* empty */ \
    else \
        err_quit(BOLD"%s:%d: "NONE"[----"L_RED"%s"NONE"----]: " \
            "Assertion '"#expr"' failed", \
            __FILE__, __LINE__, __func__); \
} while(0)

/* error related to system call*/
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

#endif
