#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1
#define NULL 0

const uint INT_LEN = sizeof(int);

int lpipe_first_data(int lpipe[2], int *dst) {
  if (read(lpipe[RD], dst, INT_LEN) == INT_LEN) {
    printf("prime %d\n", *dst);
    return 0;
  }
  return -1;
}

void transmit_data(int lpipe[2], int rpipe[2], int first) {
  int data;
  while (read(lpipe[RD], &data, INT_LEN) == INT_LEN) {
    if (data % first) write(rpipe[WR], &data, INT_LEN);
  }
}

void primes(int lpipe[2]) {
  close(lpipe[WR]);
  int first;
  if (lpipe_first_data(lpipe, &first) == 0) {
    int fd[2];
    pipe(fd);
    transmit_data(lpipe, fd, first);
    close(lpipe[RD]);
    
    if (fork() == 0) {
      primes(fd);
    } else {
      close(fd[RD]);
      close(fd[WR]);
      wait(NULL);
    }
  } else {
    close(lpipe[RD]);
  }
  exit(0);
}

int main(int argc, char const *argv[]) {
  int fd[2];
  pipe(fd);

  for (int i = 2; i <= 35; ++i) {
    write(fd[WR], &i, INT_LEN);
  }

  if (fork() == 0) {
    primes(fd);
  } else {
    close(fd[RD]);
    close(fd[WR]);
    wait(NULL);
  }
  exit(0);
}