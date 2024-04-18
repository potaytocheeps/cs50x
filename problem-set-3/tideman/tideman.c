#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
int strength_of_victory(pair candidate_pair);
void lock_pairs(void);
bool pair_can_be_locked(int winner, int loser, int target);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs and initialize preferences to 0
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Check to see if the name is valid
    for (int candidateIndex = 0; candidateIndex < candidate_count; candidateIndex++)
    {
        string currentCandidateName = candidates[candidateIndex];
        if (strcmp(currentCandidateName, name) == 0)
        {
            // Add the index value of the appropriate candidate to the ranks array
            // in the order of the voter's preference of each candidate
            ranks[rank] = candidateIndex;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // The elements in ranks[] are the indexes of each candidate in their order of appearance
    // in candidates[]

    // Iterate over ranks[] for each of the voter's candidate preferences. The last rank is
    // skipped because that candidate is not preferred over any others
    for (int i = 0; i < candidate_count - 1; i++)
    {
        int candidate_preferenced = ranks[i];

        // Compare the current candidate with every other candidate in ranks[]. This allows
        // for tracking which candidates are preferred over others, and avoids repeating pairs
        // or comparing a candidate against themselves
        for (int j = i + 1; j < candidate_count; j++)
        {
            int other_candidate = ranks[j];
            preferences[candidate_preferenced][other_candidate]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Compare each candidate with every other candidate in preferences[]
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Compare the number of times candidate i was preferred over
            // candidate j and vice versa
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int number_of_swaps = -1;

    // Use bubble sort to sort the array
    for (int i = 0, length = pair_count; i < length - 1; length--)
    {
        // If no swaps have been made, that means the array is already sorted
        if (number_of_swaps == 0)
        {
            break;
        }

        // Reset number of swaps made each iteration
        number_of_swaps = 0;

        for (int j = i; j < length - 1; j++)
        {
            if (strength_of_victory(pairs[j]) < strength_of_victory(pairs[j + 1]))
            {
                // Swap the pairs, so that the highest gets pushed to the beginning
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
                number_of_swaps++;
            }
        }
    }

    return;
}

// Declare a function to get the strength of victory for a given pair
int strength_of_victory(pair candidate_pair)
{
    return preferences[candidate_pair.winner][candidate_pair.loser];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Iterate over pairs to check whether it can be locked
    for (int i = 0; i < pair_count; i++)
    {
        if (pair_can_be_locked(pairs[i].winner, pairs[i].loser, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Determine whether a pair can be locked without creating any cycles
bool pair_can_be_locked(int winner, int loser, int target)
{
    // Base case for recursion to avoid creating a cycle
    if (target == winner)
    {
        // If the target of the original arrow is the source of the graph,
        // then the pair cannot be locked as it will create a cycle
        return false;
    }

    // Determine if the winner of this pair has lost to another candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // If there are any arrows pointing at winner, backtrack to that pair
        // of candidates to determine if a cycle will be created
        if (locked[i][winner])
        {
            return pair_can_be_locked(i, winner, target);
        }
    }

    return true;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}
