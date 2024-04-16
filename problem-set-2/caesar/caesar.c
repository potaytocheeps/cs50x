#include <cs50.h>
#include <ctype.h> // For isdigit(), isalpha() and isupper()
#include <stdio.h>
#include <stdlib.h> // For atoi(), which converts a string to an integer
#include <string.h> // For strlen()

void convert_to_ciphertext(string plaintext, int key, char ciphertext[]);
bool only_digits(string userInput);

int main(int argc, string argv[])
{
    // Make sure there is only one command-line argument passed in
    if (argc != 2)
    {
        printf("Error. You must provide one argument with the program.\n");
        return 1;
    }

    // Make sure input contains only digits
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert string to integer
    int key = atoi(argv[1]);

    // Get plaintext from user
    string plaintext = get_string("plaintext:  ");
    char ciphertext[strlen(plaintext)];

    printf("ciphertext: ");
    convert_to_ciphertext(plaintext, key, ciphertext);

    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        printf("%c", ciphertext[i]);
    }

    printf("\n");
    return 0;
}

// Check the second element of the argv array to see if it contains only digits.
// This also makes sure no negative numbers are entered
bool only_digits(string userInput)
{
    for (int i = 0, length = strlen(userInput); i < length; i++)
    {
        if (!isdigit(userInput[i]))
        {
            return false;
        }
    }

    return true;
}

// Declare function to cipher plaintext by rotating each letter by the amount provided in key.
void convert_to_ciphertext(string plaintext, int key, char ciphertext[])
{
    int currentLetterNumber = 0;
    char letterCase = 'b';

    // Traverse the plaintext string and rotate the current character by the amount provided by key
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        if (!isalpha(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
            continue;
        }

        letterCase = isupper(plaintext[i]) ? 'A' : 'a';

        currentLetterNumber = plaintext[i] - letterCase;
        currentLetterNumber += key;
        currentLetterNumber %= 26;

        ciphertext[i] = currentLetterNumber + letterCase;
    }
}
