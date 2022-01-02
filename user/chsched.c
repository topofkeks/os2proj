//
// Created by aleksa on 1/2/22.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv){
  int scheduler, ts, alpha;
  if (argc != 4) exit(1);
  for (int i = 1; i < 4; i++) {
    switch (i) {
      case 1:
        scheduler = atoi(argv[i]);
        break;
      case 2:
        ts = atoi(argv[i]);
        break;
      case 3:
        alpha = atoi(argv[i]);
      default:
        break;
    }
  }
  sched(scheduler, ts, alpha);
  exit(0);
}
