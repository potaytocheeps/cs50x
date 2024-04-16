// A program that encrypts messages using a substitution cipher.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool is_valid_key(string cipherKey);
bool has_nonalphabetic_characters(int keyLength, string cipherKey);
bool has_repeated_letters(int keyLength, string cipherKey);
char substitute_letter(char plainLetter, string cipherKey);
void print_ciphertext(string plaintext, string cipherKey);

const int NUMBER_OF_LETTERS = 26;

int main(int argc, string argv[])
{
    // Accept a single command-line argument, a case-insensitive key to use for the substitution
    if (argc != 2)
    {
        printf("Error. Only one command-line argument is allowed.\n");
        return 1;
    }

    // Assign the command-line argument passed in by the user to cipherKey
    string cipherKey = argv[1];

    // Check that key has 26 characters, all letters, and none repeated
    if (!is_valid_key(cipherKey))
    {
        printf(
            "Error. Wrong format for key. Must be 26 characters long and all must be letters.\n");
        return 1;
    }

    // Prompt user for the message to encrypt
    string plaintext = get_string("plaintext:  ");

    // Print encrypted ciphertext
    print_ciphertext(plaintext, cipherKey);

    return 0;
}

bool is_valid_key(string cipherKey)
{
    int keyLength = strlen(cipherKey);

    // Check to see that the length is the correct length
    if (keyLength != NUMBER_OF_LETTERS)
    {
        return false;
    }

    if (has_nonalphabetic_characters(keyLength, cipherKey))
    {
        return false;
    }

    if (has_repeated_letters(keyLength, cipherKey))
    {
        return false;
    }

    return true;
}

bool has_nonalphabetic_characters(int keyLength, string cipherKey)
{
    char currentLetter = 'a';

    for (int i = 0; i < keyLength; i++)
    {
        currentLetter = cipherKey[i];

        if (!isalpha(currentLetter))
        {
            return true;
        }
    }

    return false;
}

bool has_repeated_letters(int keyLength, string cipherKey)
{
    // Create array needed to check for repeated letters and initialize each element to 0
    int alphabetLetterCount[NUMBER_OF_LETTERS];

    for (int i = 0; i < keyLength; i++)
    {
        alphabetLetterCount[i] = 0;
    }

    int letterIndex = 0;
    char currentLetter = 'a';

    // Check for repeated letters in cipherKey
    for (int i = 0; i < keyLength; i++)
    {
        currentLetter = cipherKey[i];

        if (isupper(currentLetter))
        {
            letterIndex = currentLetter - 'A';
        }
        else
        {
            letterIndex = currentLetter - 'a';
        }

        alphabetLetterCount[letterIndex]++;

        if (alphabetLetterCount[letterIndex] > 1)
        {
            return true;
        }
    }

    return false;
}

char substitute_letter(char plainLetter, string cipherKey)
{
    // IF the character is not alphabetical, return the character unchanged.
    if (!isalpha(plainLetter))
    {
        return plainLetter;
    }

    // Determine the casing of the letter
    if (isupper(plainLetter))
    {
        return toupper(cipherKey[plainLetter - 'A']);
    }
    else
    {
        return tolower(cipherKey[plainLetter - 'a']);
    }
}

void print_ciphertext(string plaintext, string cipherKey)
{
    printf("ciphertext: ");

    // Substitude and print each letter one at a time
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        printf("%c", substitute_letter(plaintext[i], cipherKey));
    }

    printf("\n");
}
