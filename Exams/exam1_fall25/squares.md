# Assignment: Sorted Squares

## Overview
You are given an array of integers provided as command-line arguments, sorted in ascending (non-decreasing) order. You need to output the squares of each number, also sorted in non-decreasing order.

The starter code in `squares.c` sets up a two-pointer approach (using `left` and `right` indices) to accomplish this efficiently.

## Instructions
You are required to complete the logic for two functions in `squares.c`.

### 1. absolute(int a)
Calculate and return the absolute value of the integer `a`. For example, `absolute(-9)` should return `9`, and `absolute(9)` should return `9`.

### 2. sort_squares(int *arr, int n)
Use the provided `left` and `right` pointers to iterate over the array from both ends. Compare the absolute values of the elements at these pointers, square the larger one, and place it at the correct position in the dynamically allocated `result` array (working from the end of the array to the beginning). Once you finish populating the array, make sure to free any dynamically allocated memory to prevent memory leaks!

## Testing Examples
Run the compiled program with a sequence of numbers in ascending order to verify your logic.

**Test 1: Basic Positive Numbers**
Command: `./squares 1 2 3`
Expected Output:
1 4 9

**Test 2: Mixed Negative and Positive Numbers**
Command: `./squares -4 -1 0 3 10`
Expected Output:
0 1 9 16 100

**Test 3: All Negative Numbers**
Command: `./squares -7 -3 -1`
Expected Output:
1 9 49
