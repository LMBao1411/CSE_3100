//Do not modify starter code
//You may add helper functions if needed
#include <stdio.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
  int data;
  struct Node *next;
} Node;

// Create a new node
Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

// Insert at end
void insertEnd(Node **head, int data) {
  Node *newNode = createNode(data);
  if (!*head) {
    *head = newNode;
    return;
  }
  Node *temp = *head;
  while (temp->next)
    temp = temp->next;
  temp->next = newNode;
}

// Print list
void printList(Node *head) {
  while (head) {
    printf("%d ", head->data);
    head = head->next;
  }
  printf("\n");
}

// HELPER: free up the list
void free_list(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    head = head->next;
    free(temp);
    temp = head;
  }
}

// HELPER: reverse a list
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

// HELPER: split list into two sublists
Node *split_list(Node *head) {
    Node *slow = head;
    Node *fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node *secondHalf = slow->next;
    slow->next = NULL;   // break the list

    return secondHalf;
}

void zipList(Node **headRef) {
  // fill code here
  Node *head1 = *headRef;
	Node *head2 = split_list(*headRef);
  head2 = reverse_list(head2);
  Node *curr1 = head1;
  Node *curr2 = head2;

  while ((curr1 != NULL) && (curr2 != NULL)) {
    Node *nxt1 = curr1->next;
    Node *nxt2 = curr2->next;
    curr1->next = curr2;
    curr2->next = nxt1;
    curr1 = nxt1;
    curr2 = nxt2;
  }
}

int main(int argc, char *argv[]) {
  Node *head = NULL;

  if (argc < 2) {
    printf("Usage: %s <list of integers>\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    int val = atoi(argv[i]);
    insertEnd(&head, val);
  }

  printf("Original list:\n");
  printList(head);

  zipList(&head);

  printf("Zipped list:\n");
  printList(head);

	// fill code here
  free_list(head);    // free up the list
  return 0;
}
