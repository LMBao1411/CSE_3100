# Assignment: Linked List Calculator

## Overview
The linked lists are built in reverse order, meaning the least significant
digit is the head of the list. For example, the number 123 is stored as
3 -> 2 -> 1 -> NULL.

## Instructions
You are required to implement three functions in `calculator.c`.

### 1. addition(node* num1, node* num2)
Traverse both linked lists and add the corresponding digits. Handle any
carry-over values. Create and return a new linked list representing the sum.

### 2. subtraction(node *num1, node *num2)
Calculate `num1 - num2`. You can assume `num1` is always greater than or
equal to `num2`. Traverse the lists, subtract the digits, and handle
borrowing when a digit in `num1` is smaller than `num2`. Create and return
a new linked list for the result.

## Testing Examples
Run the compiled program with two numbers to verify your logic.

**Test 1: Basic Math**
Command: `./calculator 456 123`
Expected Output:
579
333

**Test 2: Carrying and Borrowing**
Command: `./calculator 1000 5`
Expected Output:
1005
995

**Test 3: Leading Zeros**
Command: `./calculator 105 102`
Expected Output:
207
3
