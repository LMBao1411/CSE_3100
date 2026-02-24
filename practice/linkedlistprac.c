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
    Node *new_node = createNode(v);
    new_node->next = head;
    head = new_node;
    return head;
}

void *prepend_dp(Node **head, int v) {
    Node *new_node = createNode(v);
    new_node->next = *head;
    *head = new_node;
}

Node *find_last(Node *head) {
    if (head != NULL) {
        while (head->next != NULL) {
            head = head->next;
        }
    }
    return head;
}

Node *append(Node *head, int v) {
    if (head == NULL) {
        head = createNode(v);
        return head;
    }
    Node *last = find_last(head);
    last->next = createNode(v);
    return head;
}

void *append_dp(Node **head, int v) {
    if (*head == NULL) {
        *head = createNode(v);
    }
    else {
        Node *last = find_last(*head);
        last->next = createNode(v);
    }
}

void print_list(Node *head) {
    if (head == NULL) {
        printf("List is empty");
    }
    while (head != NULL) {
        printf("%d -> ",head->value);
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

Node *find_node(Node *head, int v) {
    while (head != NULL) {
        if (head->value == v) {
            return head;
        }
        head = head->next;
    }
    return head;
}

void delete_node (Node *head, int v) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node *target = find_node(head, v);
    if (target == NULL) {
        printf("Value %d is not in the list\n", v);
        return;
    }
    if (target == head) {
        free(head);
        return;
    }

    Node *curr = head;
    while (curr->next != target) {
        curr = curr->next;
    }
    curr->next = target->next;
    free(target);
}

Node *reverse_list(Node *head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    Node *prev = NULL;
    Node *curr = head;
    while (curr != NULL) {
        Node *nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    head = prev;
    return head;
}

int main() {
    Node *list = NULL;
    Node *list_dp = NULL;
    list = append(list, 10);
    list = append(list, 20);
    list = append(list, 30);
    list = prepend(list, 5);
    print_list(list);
    printf("----------\n");
    append_dp(&list_dp, 20);
    prepend_dp(&list_dp, 10);
    append_dp(&list_dp, 30);
    append_dp(&list_dp, 40);
    print_list(list_dp);

    printf("-----DELETE NODE 20 IN BOTH LISTS------\n");
    delete_node(list, 20);
    delete_node(list_dp, 20);
    delete_node(list, 100);
    delete_node(list_dp, 100);
    print_list(list);
    printf("--------\n");
    print_list(list_dp);

    printf("--------REVERSE THE LISTS--------\n");
    list = reverse_list(list);
    list_dp = reverse_list(list_dp);
    print_list(list);
    printf("--------\n");
    print_list(list_dp);
    free_list(list);
    free_list(list_dp);
}