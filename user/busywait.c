//
// Created by aleksa on 12/12/21.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int x;
    int pid = fork();
    fork();
    for (int p = 0; p < 10; p++)
    for (int i = 0; i < 10000; i++)
        for (int j = 0; j < 10000; j++){
            x = i + j;
        }
    x++; pid = pid;
    printf("done\n");
    exit(0);
}
