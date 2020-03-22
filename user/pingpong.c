#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int parent_fd[2];
  int child_fd[2];
  char echo[256];
  int len;
  if (pipe(parent_fd) < 0)
  {
    printf("pipe error");
    exit();
  }

  if (pipe(child_fd) < 0)
  {
    printf("pipe error");
    exit();
  }

  if (fork() == 0)
  {
    printf("parent\n");
    write(child_fd[1], "ping", 5);
    while ((len = read(parent_fd[0], (void *)echo, 5)) != 0)
    {
      if(len == -1){
        sleep(10);
        continue;
      }
      echo[4] = 0;
      printf("%d: received %s, len: %d\n", getpid(), echo, len);
      sleep(10);
      write(child_fd[1], "ping", 5);
      
    }
  }
  else
  {
    printf("child\n");
    
    while ((len = read(child_fd[0], (void *)echo, 5)) != 0)
    {
      if(len == -1){
        sleep(10);
        continue;
      }
      echo[4] = 0;
      printf("%d: received %s, len: %d\n", getpid(), echo, len);
      sleep(10);
      write(parent_fd[1], "pong", 5);
      
    }
  }

  exit();
}
