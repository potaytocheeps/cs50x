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

// Number of buckets in hash table: four per letter of alphabet
const unsigned int N = 104;

// Track size of dictionary
int dictionary_size = 0;

// Hash table
node *table[N];

// Track middle of linked lists
node *middle[N];

// Track tail of linked lists
node *tail[N];

// Track whether a linked list has an even number of nodes
bool list_is_even[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Call hash function on word to get a hash value
    unsigned int index = hash(word);

    // Guard against accessing a field of a NULL pointer
    if (middle[index] == NULL)
    {
        return false;
    }

    // Declare pointers to iterate linked lists
    node *current_node = NULL;
    node *end = NULL;

    // Word is located at the middle of the linked list
    if (strcasecmp(word, middle[index]->word) == 0)
    {
        return true;
    }
    // Word is located in the first half of the linked list
    else if (strcasecmp(word, middle[index]->word) < 0)
    {
        // Start iterating from beginning of list
        current_node = table[index];

        // Only check the first half of the list
        end = middle[index];
    }
    // Word is located in the second half of the linked list
    else
    {
        // Start iterating from middle of list
        current_node = middle[index];
    }

    // Traverse the linked list located at the hash value in the array
    while (current_node != end)
    {
        // Compare each node to the word to see if it is located in the dictionary
        if (strcasecmp(word, current_node->word) == 0)
        {
            return true;
        }

        // Continue with next node in linked list
        current_node = current_node->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Get index starting from 0 of first letter in word
    unsigned int index = toupper(word[0]) - 'A';

    // There are four buckets per letter
    index *= 4;

    // Get length of word
    int word_length = strlen(word);

    // First bucket has words of length 1-4
    if (word_length > 0 && word_length <= 4)
    {
        return index;
    }
    // Second bucket has words of length 5-8
    else if (word_length > 4 && word_length <= 8)
    {
        return index + 1;
    }
    // Third bucket has words of length 9-12
    else if (word_length > 8 && word_length <= 12)
    {
        return index + 2;
    }
    // Fourth bucket has words of length 13-45
    else
    {
        return index + 3;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize array elements to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
        middle[i] = NULL;
        tail[i] = NULL;
        list_is_even[i] = false;
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
        new_node->next = NULL;

        // Copy word into node
        strcpy(new_node->word, word);

        // Hash word to obtain a hash value
        unsigned int index = hash(word);

        // If this is the first node in the list
        if (table[index] == NULL)
        {
            // Add new node into linked list and set all pointers to it
            table[index] = new_node;
            middle[index] = new_node;
            tail[index] = new_node;
        }
        // If list has an even number of nodes
        else if (list_is_even[index])
        {
            // Add new node to the end of the list
            tail[index]->next = new_node;
            tail[index] = new_node;

            // List now has an odd number of nodes
            list_is_even[index] = false;
        }
        // If list has an odd number of nodes
        else
        {
            // Add new node to the end of the list
            tail[index]->next = new_node;
            tail[index] = new_node;

            // Update the middle of the list
            middle[index] = middle[index]->next;

            // List now has an even number of nodes
            list_is_even[index] = true;
        }

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
