// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Track size of dictionary
int dictionary_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Call hash function on word to get a hash value
    unsigned int index = hash(word);

    // Traverse the linked list located at the hash value in the array
    for (node *list = table[index]; list != NULL; list = list->next)
    {
        // Compare each node to the word to see if it is located in the dictionary
        if (strcasecmp(list->word, word) == 0)
        {
            return true;
        }
    }

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

        // Increase dictionary size counter
        dictionary_size++;
    }

    // Close dictionary file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
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
