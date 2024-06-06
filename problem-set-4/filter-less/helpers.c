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
            int blue = image[row][column].rgbtBlue;
            int green = image[row][column].rgbtGreen;
            int red = image[row][column].rgbtRed;

            // Get the average of blue, green, and red
            float average = blue + green + red;
            average /= 3.0;

            // Round to nearest integer
            int gray = (int) round(average);

            // Set each pixel's colors to the updated grayscale color
            image[row][column].rgbtBlue = gray;
            image[row][column].rgbtGreen = gray;
            image[row][column].rgbtRed = gray;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
