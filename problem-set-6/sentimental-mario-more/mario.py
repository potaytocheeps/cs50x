# Prints a pyramid of a height given by user

from cs50 import get_int


def main():
    # Get height of pyramid
    height = get_height()

    # Print pyramid
    print_pyramid(height)


# Prompt user for height of pyramid
def get_height():
    while True:
        height = get_int("Enter height (Must be between 1 and 8, inclusive): ")

        # Make sure height is between 1 and 8 inclusive
        if height > 0 and height <= 8:
            return height


#  Print rows of pyramid one at a time
def print_pyramid(height):
    for current_row in range(height):
        # Print spaces for current row of left half of pyramid
        print(" " * (height - (current_row + 1)), end="")

        # Print blocks for current row of left half of pyrammid
        print("#" * (current_row + 1), end="")

        # Separate two halves of pyramid
        print("  ", end="")

        # Print blocks for current row of right half of pyramid
        print("#" * (current_row + 1))


main()
