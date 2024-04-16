#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int get_score(string word);
void display_winner(int player1Word, int player2Word);

int main(void)
{
    // Prompt user for players' words
    string player1Word = get_string("Player 1: ");
    string player2Word = get_string("Player 2: ");

    // Pass in the first word and the array to get_score to get a score
    int player1Score = get_score(player1Word);
    int player2Score = get_score(player2Word);

    display_winner(player1Score, player2Score);
}

int get_score(string word)
{
    // Initialize an array named letter-values of 26 integers to the values of each letter.
    int letterValues[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                            1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int score = 0;
    int index = 0;

    for (int i = 0, length = strlen(word); i < length; i++)
    {
        char letter = toupper(word[i]);

        if (letter >= 'A' && letter <= 'Z')
        {
            index = letter - 'A';
            score += letterValues[index];
        }
    }

    return score;
}

void display_winner(int player1Word, int player2Word)
{
    if (player1Word > player2Word)
    {
        printf("Player 1 wins!\n");
    }
    else if (player2Word > player1Word)
    {
        printf("Player 2 wins\n");
    }
    else
    {
        printf("Tie!\n");
    }
}
