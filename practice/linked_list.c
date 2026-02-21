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

void *prepend_dp(Node **head, int v) {
    printf("Address stored in head = %p\n", head);
    Node *new_node = createNode(v);
    new_node->next = *head;
    *head = new_node;
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

void *append_dp(Node **head, int v) {
    printf("Address stored in head = %p\n", head);
    if (*head == NULL) {
        *head = createNode(v);
    }
    else {
        Node *last = find_last(*head);
        last->next = createNode(v);
    }
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
    Node *list_dp = NULL;
    printf("Double pointer prepend and append\n");
    prepend_dp(&list_dp, 9);
    prepend_dp(&list_dp, 8);
    append_dp(&list_dp, 200);
    append_dp(&list_dp, 300);
    print_list(list_dp);
    free_list(list_dp);

    printf("-------------------------\n");
    
    printf("Regular linked list\n");
    Node *list = NULL;
    list = prepend(list, 8);
    prepend(list, 9);
    list = append(list, 10);
    append(list, 20);
    append(list, 30);
    print_list(list);
    free_list(list);

    return 0;
}