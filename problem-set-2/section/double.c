#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int length = 5;
    int array[length];
    array[0] = 1;

    for (int i = 1; i < length; i++)
    {
        array[i] = array[i - 1] * 2;
    }

    for (int i = 0; i < length; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");
}
