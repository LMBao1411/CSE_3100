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

// HELPER: dives to the end of the list first to print
void printSetForward(ParticleSet *p) {
    if (p == NULL) return;
    
    printSetForward(p->next); // Recursive call
    printf("%d ", p->mass);   // Print happens as the recursion "unwinds"
}

void printSet(ParticleSet *p) {
    if (p == NULL) {
        printf("Empty\n");
        return;
    }
    printSetForward(p);
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
    ParticleSet *stack = NULL;
    for (int i = 0; i < count; i++) {
        int current = particles[i];
        int survived = 1;
        
        while (stack != NULL && stack->mass > 0 && current < 0) {
            if (abs(stack->mass) < abs(current)) {
                removeFromSet(&stack);  // top dies
                continue;               // add the winning particle to the stack
            }
            else if (abs(stack->mass) == abs(current)) {
                removeFromSet(&stack);  // both die
                survived = 0;
                break;
            }
            else {
                survived = 0;  // current dies
                break;
            }
        }

        if (survived) {
            addToSet(&stack, current); // set the newly added particle to the TOP of the stack
        }
    }
    return stack;
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
