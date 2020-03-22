#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int time;

  if(argc <= 1){
    printf("sleep: you must input a time.");
    exit();
  }

  time = atoi(argv[1]);
  sleep(time);
  exit();
}
