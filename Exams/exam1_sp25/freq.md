# Assignment: First Digit Frequency

## Overview
In this assignment, you will analyze an array of unsigned integers to determine the frequency of their leading (first) digits. For example, the first digit of `425` is `4`, and the first digit of `9` is `9`. You will count how many times each digit (0-9) appears as the first digit across the entire array.

## Instructions
You are required to complete the logic for two functions in `freq.c`.

### 1. unsigned firstDigit(unsigned k)
Calculate and return the first digit of the unsigned integer `k`. Since the numbers are positive, you can isolate the first digit by continuously dividing the number by 10 until it is less than 10.

### 2. void FirstDigitCount(unsigned *a, unsigned n, unsigned freq[10])
First, ensure that the `freq` array is initialized with all zeros to avoid garbage values. Then, iterate through the provided array `a` (which has `n` elements). Use your `firstDigit` function to find the starting digit of each number, and increment the corresponding index in the `freq` array.

## Testing Examples
Run the compiled program with a sequence of numbers to verify your logic. The output will print the digit (0-9) followed by a tab and its frequency.

**Test 1: Basic Distribution**
Command: `./freq 10 15 200 2 34 3`
Expected Output:
0	0
1	2
2	2
3	2
4	0
5	0
6	0
7	0
8	0
9	0

**Test 2: Single Repeating Digit**
Command: `./freq 99 98 9 9234`
Expected Output:
0	0
1	0
2	0
3	0
4	0
5	0
6	0
7	0
8	0
9	4
