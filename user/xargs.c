#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAXSZ 512
#define NULL 0

int main(int argc, char *argv[]) {
  if (argc - 1 >= MAXARG) {
    fprintf(2, "xargs: too many arguments.\n");
    exit(1);
  }

  char buf[MAXSZ];
  char *args[argc + 1];
  int p = 0, pid;

  memset((void *)buf, '\0', MAXSZ);
  memset((void *)args, 0, argc + 1);

  for (int i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }
  args[argc] = (char *)NULL;

  /** "|" pipe
   *  close(0);
   *  dup(p[0]);
   */
  while (read(0, (void *)(buf + p), 1) > 0) {
    if (buf[p] == '\n') {
      buf[p] = '\0';
      args[argc - 1] = buf;
      pid = fork();
      if (pid < 0) {
        fprintf(2, "xargs: fork() failed.\n");
        exit(1);
      } else if (pid == 0) {
        if (exec(argv[1], args) < 0) {
          fprintf(2, "xargs: exec() failed.\n");
          exit(1);
        }
      } else {
        wait(NULL);
      }
      p = 0;
    } else {
      p++;
    }
  }
  exit(0);
}