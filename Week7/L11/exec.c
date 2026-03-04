#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("This file will run some other c file\n");
 
	execl("./calc", "./calc", "product", "1", "2", "3", "4", NULL);
  // path, argv[0], and the other argv[1], argv[2], ...
  // ONCE EXEC command successfully work, it takes over the whole process
  // everything after exec will not work
	/* printf("Done with that other file\n"); */

  return 0;
}
