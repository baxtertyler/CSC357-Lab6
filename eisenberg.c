#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    char *sharedMemory, *str1, *str2;
    pid_t pid;
    int *flag, *turn, j, index;
    enum pstate {IDLE = 1, WAITING = 2, ACTIVE = 3};

    flag = mmap(NULL, 2 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    sharedMemory = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str1 = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str2 = mmap(NULL, 1000 * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    turn = mmap(NULL, 1 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    str1 = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    str2 = "01234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556";
    j = 0;

    strcpy(sharedMemory, str1);

    turn[0] = 0;
    flag[0] = IDLE;
    flag[1] = IDLE;
    pid = fork();
    if (pid > 0) {
        /* parent */
        do {
            do {
                flag[0] = WAITING;
                index = turn[0];
                while(index != 0) {
                    if(flag[index] != IDLE) {
                        index = turn[0];
                    } else {
                        index = (index+1) % 2;
                    }
                }
                index = 0;
                while ((index < 2) && ((index == 0) || flag[index] != ACTIVE)) {
                    index++;
                }
                if ((index >= 2) && ((turn[0] == 0) || flag[turn[0]] == IDLE)) {
                    break;
                }
            } while(1 < 2);
            turn[0] = 0;
            /* critical section */
            puts(sharedMemory);
            /* end of critical section */
            index = (turn[0]+1) % 2;
            while(flag[index] == IDLE) {
                index = (index+1) % 2;
            }
            turn[0] = index;
            flag[0] = IDLE;
        } while(1 < 2);
    }
    if (pid == 0) {
        /* child */
        j = 0;
        do {
            do {
                flag[1] = WAITING;
                index = turn[0];
                while(index != 1) {
                    if(flag[index] != IDLE) {
                        index = turn[0];
                    } else {
                        index = (index+1) % 2;
                    }
                }
                index = 0;
                while ((index < 2) && ((index == 1) || flag[index] != ACTIVE)) {
                    index++;
                }
                if ((index >= 2) && ((turn[0] == 1) || flag[turn[0]] == IDLE)) {
                    break;
                }
            } while(1 < 2);
            turn[0] = 1;
            /* critical section */
            if(j % 2 == 0) {
                strcpy(sharedMemory, str1);
            } else {
                strcpy(sharedMemory, str2);
            }
            j++;
            /* end of critical section */
            index = (turn[0]+1) % 2;
            while(flag[index] == IDLE) {
                index = (index+1) % 2;
            }
            turn[0] = index;
            flag[1] = IDLE;
        } while(1 < 2);
    }
    munmap(flag, 2 * sizeof(int));
    munmap(sharedMemory, 1000 * sizeof(char));
    munmap(str1, 1000 * sizeof(char));
    munmap(str2, 1000 * sizeof(char));
    munmap(turn, 1 * sizeof(int));
    return 0;
}