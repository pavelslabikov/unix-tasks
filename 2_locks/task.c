#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Missing path to target file\n");
        return -1;
    }

    char *sleeptime = NULL;
    char *pollinterval = NULL;
    int res = 0;
    while ((res = getopt(argc, argv, "s:p:")) != -1)
    {

        switch (res)
        {
        case 's':
            sleeptime = optarg;

            break;
        case 'p':
            pollinterval = optarg;

            break;
        default:
            break;
        }
    }

    if (!sleeptime)
    {
        sleeptime = "1";
    }
    if (!pollinterval)
    {
        pollinterval = "100";
    }
    char *lockfilename = malloc(9);
    strcpy(lockfilename, argv[optind]);
    strcat(lockfilename, ".lck");
    
    struct timespec tw;
    tw.tv_nsec = atol(pollinterval) * 1000000L;
    tw.tv_sec = 0L;
    while (access(lockfilename, F_OK) == 0)
    {
        nanosleep(&tw, NULL);
        
    }
    printf("lock file not exists - acquiring lock\n");
    int lockfilefd = open(lockfilename, O_WRONLY | O_CREAT, 0777);

    int pid = getpid();
    int bytescount = ceil(log10(pid)) + 1;
    char *pidstr = malloc(bytescount);
    sprintf(pidstr, "%d", getpid());
    write(lockfilefd, pidstr, bytescount);
    close(lockfilefd);

    int isleeptime = atoi(sleeptime);
    sleep(isleeptime);

    if (access(lockfilename, F_OK) == 0)
    {

        FILE *lockfilefd = fopen(lockfilename, "r");
        fseek(lockfilefd, 0, SEEK_END);
        int sz = ftell(lockfilefd);
        char *actpidstr = malloc(sz);
        fseek(lockfilefd, 0, SEEK_SET);
        fread(actpidstr, 1, sz, lockfilefd);
        int actpid = atoi(actpidstr);
        if (actpid != getpid())
        {
            printf("Error - inconsistent pids %d != %d", actpid, getpid());
            return -1;
        }
        fclose(lockfilefd);
        if (remove(lockfilename) != 0)
        {
            printf("Error - cannot delete lck file, errno=%d", errno);
            return -1;
        }
    }
    else
    {
        printf("Error - lock file not exists when lock is actually acquired\n");
        return -1;
    }
    printf("Successfully released lock");

tw.tv_nsec = 100 * 1000000L;
    tw.tv_sec = 0L;
        nanosleep(&tw, NULL);
 
    return 0;
}
