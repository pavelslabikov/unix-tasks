#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   
   /* Работа с аргументами*/
   int res = 0;
   while ((res = getopt(argc, argv, "ab:c")) != -1) {
        switch (res)
        {
            printf("Нашли %s\n", optarg);
        case 'a':

            break;
        case 'b':
            break;
        default:
            break;
        }
   }

   /* Работа с файлами*/
   int bufsize = 4096;
   FILE *fp = fopen("1.txt", "r");
   char buf[4096];
   int bytesread;
   bytesread = fread(buf, 1, sizeof(buf), fp);
    for (int i = 0; i < sizeof(buf); i++)
    {
        printf("symbol read: %c\n", buf[i]);
    }
    
   fclose(fp);

   return 0;
}
