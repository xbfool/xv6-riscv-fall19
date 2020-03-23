#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *pattern)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    //printf("path: %s, pattern %s\n", path, pattern);
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if (!strcmp(fmtname(path), pattern))
            printf("%s\n", path);
        else
        {
            //printf("%s : %s\n", fmtname(path), pattern);
        }
        
        //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        //printf("path1: %s\n", buf);
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            //printf("path2: %s\n", buf);
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            //printf("path3: %s\n", buf);
            if (!strcmp(fmtname(buf), ".") || !strcmp(fmtname(buf), ".."))
                continue;
            //printf("path4: %s\n", buf);
            find(buf, pattern);
            //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("arg error");
        exit();
    }

    find(argv[1], argv[2]);
    exit();
}
