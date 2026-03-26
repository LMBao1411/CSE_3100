#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node {
    int v;
    struct node* next;
} node;

node* new_node(int value);
node* prepend(node* head, node* new_n);
void print_list(node* head);
void delete_list(node* head);

#endif
