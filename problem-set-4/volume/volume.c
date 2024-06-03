// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Declare a <buffer> in which to store the header
    char *header_buffer = malloc(HEADER_SIZE);

    // Copy header from input to output
    fread(header_buffer, HEADER_SIZE, 1, input);
    fwrite(header_buffer, HEADER_SIZE, 1, output);

    // Free the dynamically allocated memory for buffer
    free(header_buffer);

    // Declare a pointer to a 16-bit signed integer and dynamically allocate enough memory for it
    int16_t *sample_buffer = malloc(sizeof(int16_t));

    // Read samples from input file and write updated data to output file
    while (fread(sample_buffer, sizeof(int16_t), 1, input) != 0)
    {
        // Multiply the sample by the given factor to change its volume
        *sample_buffer *= factor;

        fwrite(sample_buffer, sizeof(int16_t), 1, output);
    }

    // Free the dynamically allocated memory for sample
    free(sample_buffer);

    // Close files
    fclose(input);
    fclose(output);
}
