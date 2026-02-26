#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

node* new_node(int value) {
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    n->v = value;
    n->next = NULL;
    return n;
}

node* prepend(node* head, node* new_n) {
    new_n->next = head;
    return new_n;
}

void print_recursive(node* head) {
    if (head == NULL) return;
    print_recursive(head->next);
    printf("%d", head->v);
}

void print_list(node* head) {
    if (head == NULL) {
        printf("0\n");
        return;
    }
    node* temp = head;
    while (temp != NULL) {
        printf("%d", temp->v);
        temp = temp->next;
    }
    printf("\n");
}

void delete_list(node* head) {
    node* current = head;
    node* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}
