# Assignment: Sorted Linked Lists

## Overview
In this assignment, you will be working with linked lists that maintain their elements in a sorted (ascending) order. You will implement logic to correctly insert a new node into a sorted list, and logic to merge two sorted lists together.

## Instructions
You are required to complete the logic for two functions in `linked_list.c`.

### 1. insert(node **head, node *newnode)
Given a double pointer to the head of a sorted linked list, insert `newnode` into its correct sorted position. Make sure to handle edge cases, such as inserting into an empty list, inserting at the very beginning (replacing the head), and inserting in the middle or end.

### 2. merge(node **p1, node **p2)
Given two sorted linked lists, merge them into a single sorted linked list and return the head of the new merged list.
* **Important:** You must use the existing nodes (rearranging their `next` pointers) rather than allocating new ones.
* **Important:** Set the original head pointers (`*p1` and `*p2`) to `NULL` before returning, as the nodes belong entirely to the new list now.

## Testing Examples
The `main` function automatically generates arrays of size `n` (provided via command line argument) and tests your functions.

**Test 1: n = 3**
Command: `./linked_list 3`
Expected Output:
0 1 2
0 2 4
0 0 1 2 2 4
(nil)
(nil)
*(Note: `(nil)` might appear as `0x0` or `00000000` depending on your operating system).*
