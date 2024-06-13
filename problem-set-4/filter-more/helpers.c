#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over every pixel
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Get values of each color
            int red = image[row][column].rgbtRed;
            int green = image[row][column].rgbtGreen;
            int blue = image[row][column].rgbtBlue;

            // Get the average of blue, green, and red
            float average = blue + green + red;
            average /= 3.0;

            // Round to nearest integer
            int gray = (int) round(average);

            // Set each pixel's colors to the updated grayscale color
            image[row][column].rgbtRed = gray;
            image[row][column].rgbtGreen = gray;
            image[row][column].rgbtBlue = gray;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over every row of pixels in the image
    for (int row = 0; row < height; row++)
    {
        // For each row, swap first and last values until the indexes cross each other
        for (int i = 0, j = width - 1; i < j; i++, j--)
        {
            // Swap values
            RGBTRIPLE temp = image[row][i];
            image[row][i] = image[row][j];
            image[row][j] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of the image
    RGBTRIPLE copy[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    // Iterate over every pixel
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Declare variables to hold the total sum of each color neighboring the current pixel
            float redSum = 0;
            float blueSum = 0;
            float greenSum = 0;
            float pixelSum = 0;

            // Iterate over the pixels neighboring the current pixel, including the current pixel
            for (int i = row - 1; i <= row + 1; i++)
            {
                for (int j = column - 1; j <= column + 1; j++)
                {
                    // Make sure the indexes are positive and that they don't go out of range
                    if ((i >= 0 && j >= 0) && (i < height && j < width))
                    {
                        // Calculate the sum of each color value for every pixel in a square shape
                        redSum += copy[i][j].rgbtRed;
                        greenSum += copy[i][j].rgbtGreen;
                        blueSum += copy[i][j].rgbtBlue;
                        pixelSum++;
                    }
                }
            }

            // Update the image's pixels with the average of the color values for each pixel
            // neighboring the current pixel
            image[row][column].rgbtRed = (int) round(redSum / pixelSum);
            image[row][column].rgbtGreen = (int) round(greenSum / pixelSum);
            image[row][column].rgbtBlue = (int) round(blueSum / pixelSum);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
