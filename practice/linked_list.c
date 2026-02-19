#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node *next;
} Node;


Node *createNode(int v) {
    Node *n = malloc(sizeof(Node));
    n->value = v;
    n->next = NULL;
    return n;
}

Node *prepend(Node *head, int v) {
    printf("Address stored in head = %p\n", head);
    Node *new_node = createNode(v);
    new_node->next = head;
    head = new_node;
    return head;
}

Node *find_last(Node *head) {
    if (head != NULL) {
        while(head->next != NULL) {
            head = head->next;
        }
    }
    return head;
}

Node *append(Node *head, int v) {
    printf("Address stored in head = %p\n", head);
    if (head == NULL) {
        return createNode(v);
    }
    Node *last = find_last(head);
    last->next = createNode(v);
    return head;
}

void print_list(Node *head) {
    if (head == NULL){
        printf("Empty list\n");
    }
    while (head!=NULL) {
        printf("%d->",head->value);
        head = head->next;
    }
    printf("NULL\n");
}

void free_list(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        head = head->next;
        free(temp);
        temp = head;
    }
}

int main() {
    Node *list = NULL;
    // list = prepend(list, 10);
    // prepend(list, 9);
    // prepend(list, 8);

    list = append(list, 100);
    append(list, 200);
    append(list, 300);
    print_list(list);
    free_list(list);
    return 0;
}