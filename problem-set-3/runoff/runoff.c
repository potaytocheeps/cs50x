#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Iterate over candidates[] to see if the given name is valid
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the given name matches a candidate's name
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Add the index value of that candidate to preferences
            preferences[voter][rank] = i;
            // Return true to indicate the vote was valid
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int candidate_index = -1;

    // Count the total number of times a candidate was a voter's first preference
    for (int voter = 0; voter < voter_count; voter++)
    {
        for (int rank = 0; rank < candidate_count; rank++)
        {
            candidate_index = preferences[voter][rank];

            // Increase candidate's total vote count if they haven't been eliminated
            if (!candidates[candidate_index].eliminated)
            {
                candidates[candidate_index].votes++;

                // Continue to the next voter
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Check if any candidate has a vote count that is the majority of the votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[i].name);

            // A candidate has the vote majority
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Hold the minimum number of votes
    int min = MAX_VOTERS + 1;

    // Find the minimmum number of votes any candidate still in the election has
    for (int i = 0; i < candidate_count; i++)
    {
        // Ignore any eliminated candidates
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Check if all remaining candidates have the same number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes != min)
            {
                // Not every remaining candidate has the same number of votes
                return false;
            }
        }
    }

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Eliminate a candidate if they have the minimum number of votes in the election
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}
