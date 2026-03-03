#include <stdio.h>

int main()
{
    int rows = 5;

    // This loop to print all rows
    for (int i = 0; i < rows; i++)
    {

        // Inner loop 1 to print white spaces for each row
        for (int j = 0; j < 2 * (rows - i) - 1; j++)
        {
            printf(" ");
        }

        // Inner loop 2 to print star (*) character for each row
        for (int k = 0; k < 2 * i + 1; k++)
        {
            printf("* ");
        }
        printf("\n");
    }
    return 0;
}