#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

int main(int argc, char const *argv[]) {
  char buf = 'P';

  int fd[2];
  pipe(fd);

  int pid = fork();
  int exit_status = 0;

  if (pid < 0) {
    fprintf(2, "fork() error!\n");
    close(fd[WR]);
    close(fd[RD]);
    exit(1);
  } else if (pid == 0) {
    if (read(fd[RD], &buf, sizeof(char)) != sizeof(char)) {
      fprintf(2, "child read() error!\n");
      exit_status = 1;
    } else {
      fprintf(1, "%d: received ping\n", getpid());
    }

    if (write(fd[WR], &buf, sizeof(char)) != sizeof(char)) {
      fprintf(2, "child write() error!\n");
      exit_status = 1;
    }

    close(fd[WR]);
    close(fd[RD]);

    exit(exit_status);
  } else {
    if (write(fd[WR], &buf, sizeof(char)) != sizeof(char)) {
      fprintf(2, "parent write() error!\n");
      exit_status = 1;
    }

    if (read(fd[RD], &buf, sizeof(char)) != sizeof(char)) {
      fprintf(2, "parent read() error!\n");
      exit_status = 1;
    } else {
      fprintf(1, "%d: received pong\n", getpid());
    }

    close(fd[WR]);
    close(fd[RD]);

    exit(exit_status);
  }
}