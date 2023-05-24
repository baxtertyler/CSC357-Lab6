#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    char *sharedMemory, *str1, *str2;
    pid_t pid;
    int *flag, *turn, j;

    flag = mmap(NULL, 2 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    sharedMemory = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str1 = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str2 = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    turn = mmap(NULL, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str1 = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    str2 = "01234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556";
    j = 0;

    flag[0] = 1;
    flag[1] = 0;
    pid = fork();
    if (pid > 0) {
        /* parent */
        do {
            flag[0] = 1;
            turn[0] = 1;
            while(flag[1] == 1 && turn[0] == 1); /* wait section */
            /* critical section */
            puts(sharedMemory);
            flag[0] = 0;
        } while(1 < 2);
    }
    if (pid == 0) {
        /* child */
        j = 0;
        do {
            flag[1] = 1;
            turn[0] = 0;
            while(flag[0] == 1 && turn[0] == 0); /* wait section */
            /* critical section */
            if(j % 2 == 0) {
                strcpy(sharedMemory, str1);
            } else {
                strcpy(sharedMemory, str2);
            }
            flag[1] = 0;
            j++;

        } while(1 < 2);
    }
    munmap(flag, 2 * sizeof(int));
    munmap(sharedMemory, 1000 * sizeof(char));
    munmap(str1, 1000 * sizeof(char));
    munmap(str2, 1000 * sizeof(char));
    munmap(turn, 1 * sizeof(int));
    return 0;
}