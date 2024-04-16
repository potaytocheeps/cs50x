#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string phrase = get_string("Enter text: ");

    for (int i = 0, length = strlen(phrase); i < length - 1; i++)
    {
        if (phrase[i] > phrase[i + 1])
        {
            printf("Not alphabetical.\n");
            return 1;
        }
    }
    printf("Is alphabetical.\n");
}
