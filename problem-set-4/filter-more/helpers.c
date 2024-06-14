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
    // Make a copy of the image
    RGBTRIPLE copy[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    // Declare 2D arrays for gx and gy grids
    int gx[3][3] = {{ -1, 0, 1 },
                    { -2, 0, 2 },
                    { -1, 0, 1 }};
    int gy[3][3] = {{ -1, -2, -1 },
                    { 0, 0, 0 },
                    { 1, 2, 1 }};

    // Iterate over every pixel of the image
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Declare variables to store gx-color sums
            int gxRedSum = 0;
            int gxGreenSum = 0;
            int gxBlueSum = 0;

            // Declare variables to store gy-color sums
            int gyRedSum = 0;
            int gyGreenSum = 0;
            int gyBlueSum = 0;

            // Loop through all pixels neighboring the current pixel
            for (int i = row - 1, gRow = 0; i <= row + 1; i++, gRow++)
            {
                for (int j = column - 1, gColumn = 0; j <= column + 1; j++, gColumn++)
                {
                    // If ranges stay within bounds, use the pixel's color values for calculations
                    if ((i >= 0 && j >= 0) && (i < height && j < width))
                    {
                        // Multiply the current pixel's color values by the corresponding values
                        // in gx and add to gxSum for each color
                        gxRedSum += copy[i][j].rgbtRed * gx[gRow][gColumn];
                        gxGreenSum += copy[i][j].rgbtGreen * gx[gRow][gColumn];
                        gxBlueSum += copy[i][j].rgbtBlue * gx[gRow][gColumn];

                        // Multiply the current pixel's color values by the corresponding values
                        // in gy and add to gySum for each color
                        gyRedSum += copy[i][j].rgbtRed * gy[gRow][gColumn];
                        gyGreenSum += copy[i][j].rgbtGreen * gy[gRow][gColumn];
                        gyBlueSum += copy[i][j].rgbtBlue * gy[gRow][gColumn];
                    }
                    // If indexes are negative or they go out of range, treat as if the pixels
                    // were black
                    else
                    {
                        // Calculate the sums for the gx and gy values
                        gxRedSum += 0 * gx[gRow][gColumn];
                        gxGreenSum += 0 * gx[gRow][gColumn];
                        gxBlueSum += 0 * gx[gRow][gColumn];

                        gyRedSum += 0 * gy[gRow][gColumn];
                        gyGreenSum += 0 * gy[gRow][gColumn];
                        gyBlueSum += 0 * gy[gRow][gColumn];
                    }
                }
            }

            // Use formula to calculate the edge detection color values for current pixel
            // The formula is: square root of (gx^2 + gy^2)
            float redValue = sqrt((gxRedSum * gxRedSum) + (gyRedSum * gyRedSum));
            float greenValue = sqrt((gxGreenSum * gxGreenSum) + (gyGreenSum * gyGreenSum));
            float blueValue = sqrt((gxBlueSum * gxBlueSum) + (gyBlueSum * gyBlueSum));

            // Round values to nearest integers
            int red = (int) round(redValue);
            int green = (int) round(greenValue);
            int blue = (int) round(blueValue);

            // Make sure no values go over 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            // Update pixel's color values with calculated edge detection value
            image[row][column].rgbtRed = red;
            image[row][column].rgbtGreen = green;
            image[row][column].rgbtBlue = blue;
        }
    }

    return;
}
