#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int calculate_number_of_letters(string text);
int calculate_number_of_words(string text);
int calculate_number_of_sentences(string text);
float calculate_reading_level(int letterCount, int wordCount, int sentenceCount);
void display_reading_level(float readingLevel);

int main(void)
{
    // Prompt user for a text input
    string text = get_string("Text: ");

    int letterCount = calculate_number_of_letters(text);
    int wordCount = calculate_number_of_words(text);
    int sentenceCount = calculate_number_of_sentences(text);

    float readingLevel = calculate_reading_level(letterCount, wordCount, sentenceCount);

    display_reading_level(readingLevel);
}

// Declare a function to get the total number of letters in the text.
int calculate_number_of_letters(string text)
{
    int letterCount = 0;

    for (int index = 0; text[index] != '\0'; index++)
    {
        if (isalpha(text[index]))
        {
            letterCount++;
        }
    }

    return letterCount;
}

// Declare a function to get the total number of words in the text
int calculate_number_of_words(string text)
{
    int wordCount = 0;

    for (int index = 0; text[index] != '\0'; index++)
    {
        if (isspace(text[index]))
        {
            wordCount++;
        }
    }

    // Include the last word in the text, which will be skipped by the loop
    wordCount++;

    return wordCount;
}

// Declare a function to get the total number of sentences in the text
int calculate_number_of_sentences(string text)
{
    int sentenceCount = 0;

    for (int index = 0; text[index] != '\0'; index++)
    {
        if (text[index] == '.' || text[index] == '?' || text[index] == '!')
        {
            sentenceCount++;
        }
    }

    return sentenceCount;
}

// Declare a function to get the reading level of a text
float calculate_reading_level(int letterCount, int wordCount, int sentenceCount)
{
    float L = ((float) letterCount / (float) wordCount) * 100.0;
    float S = ((float) sentenceCount / (float) wordCount) * 100.0;

    float readingLevel = 0.0588 * L - 0.296 * S - 15.8;

    return readingLevel;
}

void display_reading_level(float readingLevel)
{
    int grade = rintf(readingLevel);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
