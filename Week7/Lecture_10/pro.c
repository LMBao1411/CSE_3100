#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("This is a demonstration\n");
  int a = fork();
  if (a == 0) {
    printf("This is in child process\n");
    printf("Child process id = %d\n", getpid());
  }
  if (a > 0) {
    printf("This is in parent process\n");
    waitpid(a, NULL, 0);
    printf("Parent process id = %d\n", getpid());
  }

  printf("Bye!\n");
  return 0;
}

// the child process only run after fork(), and the child has the same processes as the parent
// and line 18 the child can run it because there is no condition
// child process a is == 0, parent process a is > 0.
// child's pid is equal to the a value in parent process