#ifndef lib_error_h
#define lib_error_h

#include <errno.h>

#define THROW_EXCEPTION(msg) \
    err_sys("[----%s----]: "msg, __func__)
#define THROW_WARNING(msg) \
    err_msg("[----%s----]: "msg, __func__)

/* error related to system call*/
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

#endif
