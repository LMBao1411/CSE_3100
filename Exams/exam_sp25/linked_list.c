#include "linked_list.h"

// create a node with value v
node* create_node(int v)
{
    node *p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);

    // Set the value in the node.
    p->v = v;
    p->next = NULL;
    return p;
}

// TODO: Insert newnode into the sorted linked list
void insert(node **head, node *newnode)
{
    // Write your logic here to insert `newnode` into the correct position
    // so that the linked list remains sorted in ascending order.

}

// add newnode to the end of the linked list determined by *head and *tail
void insert_last(node **head, node **tail, node *newnode)
{
	if((*head) == NULL)
	{
		(*head) = (*tail) = newnode;
	}
	else
	{
		(*tail)->next = newnode;
		(*tail) = newnode;
	}
}

// remove the first node from the list and return a pointer to it
node* remove_first(node **head)
{
    node *p = (*head);
    if((*head) != NULL) (*head) = (*head)->next;
    return p;
}

// print the list pointed by head
void print_list(node *head)
{
	node *p = head;
	while(p != NULL)
	{
		printf("%d ", p->v);
		p = p->next;
	}
	printf("\n");
}

// TODO: Merge two sorted linked lists into one
// Return the head of the merged linked list
node* merge(node **p1, node **p2)
{
    node *head = NULL;

    // Write your logic here to merge list p1 and list p2.
    // The merged list should be sorted.
    // Remember to set *p1 and *p2 to NULL at the end!

    return head;
}

int main(int argc, char *argv[])
{
	assert(argc == 2);
	int n = atoi(argv[1]);

	int a[n];
	int b[n];

	for(int i = 0; i < n; i++)
	{
		a[i] = i;
		b[i] = 2*i;
	}

	node *p1 = NULL, *p2 = NULL;

	for(int i = 0; i < n; i++)
	{
		node *p = create_node(a[i]);
 		insert(&p1, p);
	}
	print_list(p1);

	for(int i = 0; i < n; i++)
    {
        node *p = create_node(b[i]);
        insert(&p2, p);
    }
	print_list(p2);

	node *p = merge(&p1, &p2);
	print_list(p);

    // These should print as (nil) or 0x0 since the pointers should be NULLed
	printf("%p\n", (void *)p1);
	printf("%p\n", (void *)p2);

    // Free the dynamically allocated memory
	node *current = p;
	while (current != NULL) {
		node *next = current->next;
		free(current);
		current = next;
	}

	return 0;
}
