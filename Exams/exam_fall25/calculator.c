#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linkedlist.h"

// HELPER: reverse a list
node *reverse_list(node *head) {
  if (head == NULL || head->next == NULL) {
        return head;
    }
    node *prev = NULL;
    node *curr = head;
    while (curr != NULL) {
        node *nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    head = prev;
    return head;
}

// HELPER: convert list to an integer
int list_to_int(node *head) {
    int num = 0;
    int digit = 1;
    while (head) {
        num += (head->v) * digit;
        digit *= 10;
        head = head->next;
    }
    return num;
}

// HELPER: reverse a number
int reverseNum(int num) {
    int rev_num = 0;
    while (num > 0) {
        rev_num = rev_num * 10 + num % 10;
        num /= 10;
    }
    return rev_num;
}

node* addition(node* num1, node* num2) {
    node* result = NULL;
    // TODO: Implement addition logic here
    // Convert lists to integers
    long int int1 = list_to_int(num1);
    long int int2 = list_to_int(num2);
    long int sum = int1 + int2;
    if (sum == 0) {
        node* zeroNode = new_node(0);
        result = prepend(result, zeroNode);
        return result;
    }
    while (sum > 0) {
        int digit = sum % 10;
        sum /= 10;
        node* newNode = new_node(digit);
        result = prepend(result, newNode);
    }
    return result;
}

node* subtraction(node *num1, node *num2) {
    node *result = NULL;

    // TODO: Implement subtraction logic here
    long int int1 = list_to_int(num1);
    long int int2 = list_to_int(num2);
    long int diff = int1 - int2;
    if (diff == 0) {
        node* zeroNode = new_node(0);
        result = prepend(result, zeroNode);
        return result;
    }
    while (diff > 0) {
        int digit = diff % 10;
        diff /= 10;
        node* newNode = new_node(digit);
        result = prepend(result, newNode);
    }
    return result;
}

node *remove_leading_zeros(node *num){
    node *temp = num;
    while(temp!=NULL && temp->v == 0){
        node *temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    return temp;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./sum number1 number2\n");
        exit(0);
    }

    node *num1 = NULL, *num2 = NULL;
    node *sum = NULL;
    node *difference = NULL;

    for (int i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            printf("Invalid input in number1\n");
            exit(0);
        }
        num1 = prepend(num1, new_node(argv[1][i] - '0'));
    }

    for (int i = 0; i < strlen(argv[2]); i++) {
        if (!isdigit(argv[2][i])) {
            printf("Invalid input in number2\n");
            exit(0);
        }
        num2 = prepend(num2, new_node(argv[2][i] - '0'));
    }

    sum = addition(num1, num2);
    difference = subtraction(num1, num2);

    sum = remove_leading_zeros(sum);
    difference = remove_leading_zeros(difference);

    print_list(sum);
    print_list(difference);

    delete_list(num1);
    delete_list(num2);
    delete_list(sum);
    delete_list(difference);

    return 0;
}
