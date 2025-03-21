#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void signal_handler(int signum)
{
    if (signum == SIGUSR1)
        printf("J'ai eu un signal\r\n");
    return;
}

int main()
{FILE *f;
 printf("Penser a mknod /dev/jmf c 91 0 avant de lancer ce programme\n");
    signal(SIGUSR1, signal_handler);
    printf("My PID is %d.\n", getpid());
    f=fopen("/dev/jmf","w");   
    fprintf(f,"%d\n", getpid()); fflush(f);
    fclose(f);
    printf("send: %d\n", getpid());
    while (1) {};
    return 0;
}
