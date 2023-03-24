#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{

    int fd = open("fileA", O_CREAT | O_TRUNC | O_WRONLY, 0777);
    truncate("fileA", 4 * 1024 * 1024 + 1);
    lseek(fd, 0, SEEK_SET);
    write(fd, "1", 1);
    lseek(fd, 10000, SEEK_SET);
    write(fd, "1", 1);
    lseek(fd, 0, SEEK_END);
    write(fd, "1", 1);
    close(fd);
    return 0;
}