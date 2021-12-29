/*
 * Error wrapper functions header
 */
#ifndef wrapper_error_h
#define wrapper_error_h

#include <errno.h>

#define DEBUG_TRACE(msg)     \
    err_msg("[----%s----]: " \
        msg,                 \
        __func__);
#define SYS_QUIT(msg)        \
    err_sys("[----%s----]: " \
        msg,                 \
        __func__);

/* error related to system call*/
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

#endif
