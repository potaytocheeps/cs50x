// Recovers JPEGs from a forensic image

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Create a global definition of an unsigned integer of 8 bits as a BYTE
typedef uint8_t BYTE;
// The forensic image is in FAT file format, so each block of memory is 512 bytes
const int BLOCK_SIZE = 512;
const int FILENAME_SIZE = 8;

int main(int argc, char *argv[])
{
    // Accept exactly one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // Store input file name
    char *in_filename = argv[1];

    // Open the file given as an argument for reading
    FILE *input_file = fopen(in_filename, "r");
    if (input_file == NULL)
    {
        printf("%s could not be opened.\n", in_filename);
        return 1;
    }

    // Create a buffer to store the information from the memory card
    BYTE buffer[BLOCK_SIZE];

    // Declare counter for number of jpegs found to be used in output file names
    int jpegs_found = 0;

    // Allocate memory for the name of the output file.
    char *out_filename = malloc(sizeof(char) * FILENAME_SIZE);

    // Declare file pointer for output file
    FILE *output_file = NULL;

    // Read information from the input file into the buffer
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, input_file) != 0)
    {
        // Check the first four bytes in buffer to determine whether it is a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            // If a jpeg file had previously been found, close that file
            if (output_file)
            {
                fclose(output_file);
            }

            // Name the JPEG file
            sprintf(out_filename, "%03i.jpg", jpegs_found);

            // Open output file for writing
            output_file = fopen(out_filename, "w");
            if (output_file == NULL)
            {
                fclose(input_file);
                printf("%s could not be created.\n", out_filename);
                return 1;
            }

            // Increase the jpeg counter
            jpegs_found++;

            // Write information in buffer into output file
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output_file);
        }
        else
        {
            // If already found JPEG, continue writing to it
            if (output_file)
            {
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output_file);
            }
        }
    }

    // Free memory used for output file name
    free(out_filename);

    // Close files
    fclose(input_file);
    fclose(output_file);
}
