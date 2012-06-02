#include <stdio.h>
#include <signal.h>
#include <errno.h>
void checkmask(void);
int errno_backup;

int main(void)
{
    char buffer[100]; 
    sigset_t blockset;
    errno_backup=errno;
    sigemptyset(&blockset);
    sigaddset(&blockset,SIGINT);
    sigaddset(&blockset,SIGTSTP);
    sigprocmask(SIG_SETMASK,&blockset,NULL);
    checkmask();
    printf("press any key to continue,");
    getchar();
    sigaddset(&blockset,SIGTERM);
    sigprocmask(SIG_BLOCK,&blockset,NULL);
    checkmask();
    printf("press any key to continue.");
    getchar();
    sigdelset(&blockset,SIGINT);
    sigprocmask(SIG_UNBLOCK,&blockset,NULL);
    checkmask();
    printf("press any key to finish.");
    getchar();
}
void checkmask(void)
{
    sigset_t sigset;
    if(sigprocmask(0,NULL,&sigset)<0)
    {
        printf("sigprocmask error\n");
        errno=errno_backup;
        exit(0);
    }
    printf("signal mask:");

    if(sigismember(&sigset,SIGINT))
        printf("SIGINT ");

    if(sigismember(&sigset,SIGTSTP))
        printf("SIGTSTP ");

    if(sigismember(&sigset,SIGTERM))
        printf("SIGTERM ");

    printf("\n");

    errno=errno_backup;
}

