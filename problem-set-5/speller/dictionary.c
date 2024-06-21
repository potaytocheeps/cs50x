// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Call hash function on word to get a hash value.
    // Use the hash value to access the bucket in the array in which this word should be located.
    // Traverse the linked list and compare each node to the word to see if it has been found
    // in the dictionary. If I get to the end, and the word wasn't found, then it is not in the
    // dictionary. Return false if so. Otherwise, return true if found.
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize array elements to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    // Check if it returns NULL
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Declare char array to read words into
    char word[LENGTH + 1];

    // Read strings from file one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *new_node = malloc(sizeof(node));
        // Check to see if it returns null
        if (new_node == NULL)
        {
            printf("Could not allocate memory for new node.\n");
            return false;
        }

        // Copy word into node
        strcpy(new_node->word, word);

        // Hash word to obtain a hash value
        unsigned int index = hash(word);

        // Add new node into linked list
        new_node->next = table[index];
        table[index] = new_node;
    }

    // Close dictionary file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // Keep track of the size of the dictionary as it is being loaded with a global variable
    // Return the value of the global variable keeping track of the size
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Declare temporary pointer to a node
    node *current_node = NULL;

    // Traverse the array of linked lists
    for (int i = 0; i < N; i++)
    {
        // Iterate over linked list as long as there are still nodes in it
        while (table[i] != NULL)
        {
            // Free the current node
            current_node = table[i];
            table[i] = table[i]->next;
            free(current_node);
        }
    }

    return true;
}
