#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>      /* for definition of errno */
#include <stdarg.h>     /* ISO C variable aruments */
#include <string.h>

static void err_doit(int, int, const char *, va_list);

/*
 *  * Nonfatal error related to a system call.
 *   * Print a message and return.
 *    */
void err_ret(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}


/*
 *  * Fatal error related to a system call.
 *   * Print a message and terminate.
 *    */
void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}


/*
 *  * Fatal error unrelated to a system call.
 *   * Error code passed as explict parameter.
 *    * Print a message and terminate.
 *     */
void err_exit(int error, const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
    exit(1);
}


/*
 *  * Fatal error related to a system call.
 *   * Print a message, dump core, and terminate.
 *    */
void err_dump(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    abort();        /* dump core and terminate */
    exit(1);        /* shouldn't get here */
}


/*
 *  * Nonfatal error unrelated to a system call.
 *   * Print a message and return.
 *    */
void err_msg(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
}


/*
 *  * Fatal error unrelated to a system call.
 *   * Print a message and terminate.
 *    */
void err_quit(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}


/*
 *  * Print a message and return to caller.
 *   * Caller specifies "errnoflag".
 *    */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char    buf[1024];
    vsnprintf(buf, 1024, fmt, ap);
    if (errnoflag)
        snprintf(buf+strlen(buf), 1024-strlen(buf), ": %s",
                strerror(error));
    strcat(buf, "\n");
    fflush(stdout);     /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(NULL);       /* flushes all stdio output streams */
}

volatile sig_atomic_t quitflag;

int main(void)
{
    void sig_int(int);
    sigset_t newmask,oldmask,zeromask;
    if(signal(SIGINT,sig_int)==SIG_ERR)
        err_sys("signal (SIGINT) error");
    if(signal(SIGQUIT,sig_int)==SIG_ERR)
        err_sys("signal (SIGQUIT) error");
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK error");
    while(quitflag==0)
        sigsuspend(&zeromask);
    quitflag=0;
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        exit(0);
}
void sig_int(int signumber)
{
    if(signumber==SIGINT)
        printf("\nintetrreupt\n");
    else if(signumber==SIGQUIT)
        quitflag=1;
    return;
}

