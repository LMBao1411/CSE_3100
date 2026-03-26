#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    int v;
    struct node* next;
} node;

node* create_node(int v);
void insert(node **head, node *newnode);
void insert_last(node **head, node **tail, node *newnode);
node* remove_first(node **head);
void print_list(node *head);
node* merge(node **p1, node **p2);

#endif
