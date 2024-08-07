# Checks if a DNA sequence matches that of any individual from a database

import csv
import sys


def main():

    # Check for correct command-line usage
    if len(sys.argv) != 3:
        print("Error: Failed to provide correct number of command-line arguments.")
        print("Usage: python dna.py database.csv sequence.txt")
        sys.exit(1)

    # Store names of files provided
    database_file = sys.argv[1]
    sequence_file = sys.argv[2]

    # Get database data from file
    result = read_database(database_file)

    # Store database data and STR sequences
    database = result[0]
    subsequences = result[1]

    # Read DNA sequence file into a variable
    sequence = read_sequence(sequence_file)

    # Find longest match of each STR in DNA sequence
    sequence_data_count = get_str_count(subsequences, sequence)

    # Check database for matching profiles
    check_for_match(database, subsequences, sequence_data_count)

    return


# Checks if any individuals in database match the DNA sequence
def check_for_match(database, subsequences, sequence_data_count):

    # Check each person in database for matching profiles
    for person in database:

        # Iterate over each STR and compare data of person to data of DNA sequence
        for subsequence in subsequences:

            # If any of the current person's STR count does not match, continue to next person
            if int(person[subsequence]) != sequence_data_count[subsequence]:
                break

        # If all STR counts match for current person, then DNA sequence has been identified
        else:
            print(person["name"])
            break

    # There was no match in database for DNA sequence
    else:
        print("No match")

    return


# Returns dictionary of counts of each STR found in DNA sequence
def get_str_count(subsequences, sequence):

    sequence_data_count = {}

    # Find longest match of each STR in DNA sequence
    for subsequence in subsequences:

        sequence_data_count[subsequence] = longest_match(sequence, subsequence)

    return sequence_data_count


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


def read_database(database_file):

    # Initialize lists
    database = []
    subsequences = []

    # Open database file
    with open(database_file) as file:

        # Read database file into a variable
        reader = csv.DictReader(file)

        # Store STR sequences, excluding the 'name' fieldname
        subsequences = reader.fieldnames[1:]

        # Store data of each indidivual
        for row in reader:
            database.append(row)

    # Return list of data read from database file
    return [database, subsequences]


# Gets DNA sequence data from file
def read_sequence(sequence_file):

    sequence = ""

    with open(sequence_file) as file:

        sequence = file.read()

    return sequence


main()
