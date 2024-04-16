#include <cs50.h>
#include <stdio.h>

int get_pyramidHeight(void);
void printPyramid(int pyramidHeight);
void printRow(int numberOfSpaces, int numberOfBricks);

int main(void)
{
    int pyramidHeight = get_pyramidHeight();
    printPyramid(pyramidHeight);
}

int get_pyramidHeight(void)
{
    int pyramidHeight = 0;
    do
    {
        // Prompt the user for the pyramid's height
        pyramidHeight =
            get_int("Enter a positive integer value, greater than 0, for the pyramid's height: ");
    }
    while (pyramidHeight < 1);

    return pyramidHeight;
}

void printPyramid(int pyramidHeight)
{
    for (int row = 0; row < pyramidHeight; row++)
    {
        // Print row of bricks
        printRow(pyramidHeight - (row + 1), row + 1);
    }
}

void printRow(int numberOfSpaces, int numberOfBricks)
{
    // Print spaces
    for (int space = 0; space < numberOfSpaces; space++)
    {
        printf(" ");
    }

    // Print bricks
    for (int brick = 0; brick < numberOfBricks; brick++)
    {
        printf("#");
    }

    printf("\n");
}
