# Checks the reading level of a piece of text using the Coleman-Liau index

from cs50 import get_string


def main():
    # Get text input from user
    text = get_string("Text: ")

    # Initialize counters
    letter_count = 0
    word_count = 0
    sentence_count = 0

    # Iterate over the text input to count the number of letters, words, and sentences
    for character in text:

        # Count all alphabetic letters
        if character.isalpha():
            letter_count += 1

        # Count all words
        elif character.isspace():
            word_count += 1

        # Count all sentences
        elif character in ['.', '?', '!']:
            sentence_count += 1

    # Add one to word_count to account for the last word in the sentence
    word_count += 1

    # Calculate the reading level
    reading_level = calculate_reading_level(letter_count, word_count, sentence_count)

    # Print the reading level
    print_reading_level(reading_level)


# Calculates reading level using the Coleman-Liau index
def calculate_reading_level(letter_count, word_count, sentence_count):
    # Calculate the average number of letters per 100 words
    L = 100 * (letter_count / word_count)

    # Calculate the average number of sentences per 100 words
    S = 100 * (sentence_count / word_count)

    # Use Coleman-Liau index to compute the reading level of the text input
    return round(0.0588 * L - 0.296 * S - 15.8)


# Prints reading level of text input
def print_reading_level(reading_level):
    if reading_level < 1:
        print(f"Before Grade 1")
    elif reading_level >= 16:
        print(f"Grade 16+")
    else:
        print(f"Grade {reading_level}")


main()
