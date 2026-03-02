#include <stdio.h>
#include <stdlib.h>

typedef struct particle {
  int mass;
  struct particle *next;
} ParticleSet;

void addToSet(ParticleSet **p, int idx) {
  ParticleSet *newNode = malloc(sizeof(ParticleSet));
    newNode->mass = idx;
    newNode->next = *p;
    *p = newNode;
}

void removeFromSet(ParticleSet **p) {
  if (*p == NULL) {return;}
  ParticleSet *curr = *p;
  *p = (*p)->next;
  free(curr);
}

int isSetEmpty(ParticleSet *p) {
  if (p == NULL) {return 1;}
  else {return 0;}
}

int topOfTheSet(ParticleSet *p) {
  return p->mass;
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
  ParticleSet *survived_stack = NULL;
  for (int i = 0; i < count; i++) {
    int current = particles[i];
    int alive = 1;

    // collision condition needs to be: 
    // 1. survived stack is not null
    // 2. the particle in the stack needs to be moving to the right (+positive numbers)
    // 3. incomding particle (which is current) needs to be moving to the left
    // this means that the first particle will always be added to the stack, but will be fighting later

    // flow: each iteration, top of the stack will be fighting with current
    // if current wins, top of the stack will be removed, current will be added to the stack as the new top
    // if current and stack both die, top of the stack will be removed, nothing is added so move onto the next iteration
    // if current cannot beat stack, nothing gets added to the stack

    while (survived_stack != NULL && survived_stack->mass > 0 && current < 0) {
      if (abs(survived_stack->mass) < abs(current)) {
        removeFromSet(&survived_stack);   // the top of the survived stack dies
        continue;                         // this means the winning particle needs to be added to the stack
      }
      else if (abs(survived_stack->mass) == abs(current)) {
        removeFromSet(&survived_stack);   // both current and top of the survied stack dies
        alive = 0;                        // nothing will be added to the survived stack
        break;
      }
      else if (abs(survived_stack->mass) > abs(current)) {
        alive = 0;                        // current particle dies
        break;
      }
    }
    if (alive) {
      addToSet(&survived_stack, current);
    }
  }
    return survived_stack;
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
