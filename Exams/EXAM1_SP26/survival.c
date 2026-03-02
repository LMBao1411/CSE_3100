#include <stdio.h>
#include <stdlib.h>

typedef struct particle {
  int mass;
  struct particle *next;
} ParticleSet;

void addToSet(ParticleSet **p, int idx) {
  ParticleSet *new_p;
  new_p->next = idx;
}

void removeFromSet(ParticleSet **p) {
  ParticleSet *curr = *p;
  *p = (*p)->next;
  free(curr);
}

int isSetEmpty(ParticleSet *p) {
  if (p == NULL) {return 1;}
  else {return 0;}
}

int topOfTheSet(ParticleSet *p) {
  return p;
}

void printSet(ParticleSet *p) {
  if (isSetEmpty(p)) {
    printf("Empty\n");
    return;
  }
  while (p != NULL) {
    printf("%d ", p->mass);
    p = p->next;
  }
  printf("\n");
}

void freeSet(ParticleSet **b) {
  ParticleSet *temp = *b;
  while (temp != NULL) {
    *b = (*b)->next;
    free(temp);
    temp = *b;
  }
}

ParticleSet *fight(int *particles, int count) {
  
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./survival <particle values>\n");
    exit(1);
  }

  int count = argc - 1;
  int *particles = malloc(count * sizeof(int));

  for (int i = 0; i < count; i++) {
    particles[i] = atoi(argv[i + 1]);
  }

  ParticleSet *b = fight(particles, count);
  printSet(b);
  freeSet(&b);
	return 0;
}
