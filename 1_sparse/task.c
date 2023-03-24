#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>

char *getconcat(int offset, int zerocount)
{
    char *soffset = malloc(offset == 0 ? 1 : (int)log10(offset) + 1);
    char *szero = malloc((int)log10(zerocount) + 1);
    sprintf(soffset, "%d", offset);
    sprintf(szero, "%d", zerocount);
    return strcat(strcat(soffset, " "), strcat(szero, "\n"));
}

bool iszeroed(char buf[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (buf[i] != '\0')
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Missing path to target file\n");
        return -1;
    }

    // вычленияем размер буфера
    char *bufsize = NULL;
    int res = 0;
    while ((res = getopt(argc, argv, "b:")) != -1)
    {

        switch (res)
        {
        case 'b':
            bufsize = optarg;

            break;
        default:
            break;
        }
    }

    if (!bufsize)
    {
        char defbuf[] = "4096";
        bufsize = defbuf;
    }

    // читаем с stdin, если первый позиционный - последний
    int sourcefd;
    int targetfd;
    int targetmetafd;
    if (optind < argc - 1)
    {
        sourcefd = open(argv[optind], O_RDONLY);
        targetfd = open(argv[optind + 1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        targetmetafd = open(strcat(argv[optind + 1], ".meta"), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    }
    else
    {
        sourcefd = STDIN_FILENO;
        targetfd = open(argv[optind], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        targetmetafd = open(strcat(argv[optind], ".meta"), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    }

    int ibufsize = atoi(bufsize);

    char buf[ibufsize];
    int bytesRead = read(sourcefd, buf, ibufsize);

    int zerocount = 0;
    int offset = 0;
    int overallbytes = 0;
    while (bytesRead != 0)
    {
        overallbytes += bytesRead;
        if (iszeroed(buf, bytesRead))
        {
            if (zerocount == 0)
            {
                offset = overallbytes - bytesRead;
            }
            zerocount += bytesRead;
        }
        else
        {
            write(targetfd, buf, bytesRead);
            if (zerocount > 0)
            {
                int len = offset == 0 ? 1 : (int)log10(offset) + 1;
                len += (int)log10(zerocount) + 3;
                char *strtowrite = getconcat(offset, zerocount);
                write(targetmetafd, strtowrite, len);
            }
            zerocount = 0;
        }
        bytesRead = read(sourcefd, buf, ibufsize);
    }
    if (zerocount > 0)
    {
        int len = offset == 0 ? 1 : (int)log10(offset) + 1;
        len += (int)log10(zerocount) + 3;
        char *strtowrite = getconcat(offset, zerocount);
        write(targetmetafd, strtowrite, len);
    }
    close(sourcefd);
    close(targetfd);
    close(targetmetafd);

    return 0;
}
