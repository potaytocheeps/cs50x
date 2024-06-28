# Checks whether a credit card number is a valid American Express, MasterCard,
# or Visa card number

from cs50 import get_string


def main():
    # Get card number from user
    card_number = get_string("Number: ")

    # Determine the type of card
    card_type = get_card_type(card_number)

    # If card is invalid, exit program early
    if card_type == "INVALID":
        print(card_type)
        exit(0)

    # Check if the checksum calculation yields a valid credit card number
    card_is_valid = calculate_checksum(card_number)

    # Print type of card, if valid
    if card_is_valid:
        print(card_type)
    else:
        print("INVALID")


# Determine whether the card number is an AMEX, MasterCard, Visa, or invalid number
def get_card_type(card_number):
    if card_number.startswith("4") and len(card_number) in [13, 16]:
        card_type = "VISA"
    elif card_number.startswith(("34", "37")) and len(card_number) == 15:
        card_type = "AMEX"
    elif card_number.startswith(("51", "52", "53", "54", "55")) and len(card_number) == 16:
        card_type = "MASTERCARD"
    else:
        card_type = "INVALID"

    return card_type


# Check that the card number is a valid credit card number with a checksum calculation
def calculate_checksum(card_number):
    # Hold the sum of all digits
    sum = 0

    # Iterate over every other card digit, starting from the second-to-last digit
    for digit in card_number[-2::-2]:
        # For each digit, multiply it by 2
        result = int(digit) * 2

        # Get the sum of the individual digits of each resulting calculation
        if result >= 10:
            # Separate each digit into its own string in a list
            result = list(str(result))
            # Add each digit to total sum
            for digit in result:
                sum += int(digit)
        else:
            sum += result

    # Add the remaining digits that were not multiplied by 2 to the total sum
    for digit in card_number[-1::-2]:
        sum += int(digit)

    # Check if sum is divisible by 10
    return sum % 10 == 0


main()
