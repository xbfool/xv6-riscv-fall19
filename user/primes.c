#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int prime(int i)
{
    int p[2];
//    char buf[5];
//    int b[1];
//    int tmp;
    pipe(p);
    //printf("\t\t\tpipe: %d %d\n", p[0], p[1]);
    int num;
 //   buf[4] = 0;
    //printf("new num %d\n", i);
    //sleep(10);
    int len;
    if (fork() == 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        int fd;
        printf("is prime %d\n", i);
        fd = -1;
        close(p[1]);
        //close(0);
        while ((len = read(0, &num, sizeof(num))))
        {
            if(len == -1){
                continue;
            }
            //printf("pid: %d read %d\n", getpid(), num);
            if (num == 0)
            {
                if (fd != -1)
                {
                    write(fd, (&num), sizeof(num));
                    wait();
                }
                exit();
            }
            else if (num % i == 0)
            {
                continue;
            }
            else
            {
                if (fd == -1)
                {
                    fd = prime(num);
                    //printf("pid: %d prime %d\n", getpid(), num);
                }
                else
                {
                    //printf("write %d\n", num);
                    //printf("pid: %d write %d\n", getpid(), num);
                    write(fd, (&num), sizeof(num));
                }
            }
        }
        return 0;
    }
    else
    {
        return p[1];
    }
}

int main(int argc, char *argv[])
{
    int fd;
    int i;
    fd = prime(2);;
    for (i = 3; i < 100; i++)
    {
        //printf("num: %d\n", i);
        write(fd, (&i), sizeof(i));
    }
    i = 0;
    //write(fd, (&i), sizeof(i));
    wait();
    exit();
}
