// Recovers JPEGs from a forensic image

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Create a global definition of an unsigned integer of 8 bits as a BYTE
typedef uint8_t BYTE;
// The forensic image is in FAT file format, so each block of memory is 512 bytes
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Accept exactly one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // Store input file name
    char *input_filename = argv[1];

    // Open the file given as an argument for reading
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        printf("%s could not be opened.\n", input_filename);
        return 1;
    }

    // Create a buffer to store the information from the memory card
    BYTE buffer[BLOCK_SIZE];

    // Read information from the input file into the buffer
    int jpegs_found = 0;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, input_file) != 0)
    {
        // Check the first four bytes in buffer to determine whether it is a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            // IF it is a JPEG
            // IF this is first JPEG file found, open a new file for writing information into
            // ELSE, close the currently opened output file and open a new one for writing
            jpegs_found++;
        }

        // ELSE
            // IF already found JPEG, continue writing to it
                // I can check if a JPEG has been found if an output file exists. If it doesn't,
                // then no JPEG file has yet been found and the program can continue reading
                // information from the input file
    }

    // Close files
    fclose(input_file);

    printf("Found %i images\n", jpegs_found);
}
