# Calculates minimum amount of coins needed to produce cash owed to customer

from cs50 import get_float

# Declare constant values for each available coin
QUARTER_VALUE = 0.25
DIME_VALUE = 0.10
NICKEL_VALUE = 0.05
PENNY_VALUE = 0.01


def main():
    # Get amount of change owed
    change_amount = get_cash_owed()

    # Print amount of coins owed
    print(calculate_coins_owed(change_amount))


# Prompt user for total amount of change owed
def get_cash_owed():
    while True:
        change_amount = get_float("Change: ")

        # Make sure the input is positive
        if change_amount >= 0:
            return change_amount


# Calculate the minimum number of coins needed to be returned to customer
def calculate_coins_owed(change_amount):
    coins_owed = 0

    while change_amount > 0:

        if change_amount >= QUARTER_VALUE:
            coins_owed += 1
            change_amount -= QUARTER_VALUE

        elif change_amount >= DIME_VALUE:
            coins_owed += 1
            change_amount -= DIME_VALUE

        elif change_amount >= NICKEL_VALUE:
            coins_owed += 1
            change_amount -= NICKEL_VALUE

        else:
            coins_owed += 1
            change_amount -= PENNY_VALUE

        # Round remaining change_amount to avoid float calculation imprecisions
        change_amount = round(change_amount, 2)

    # Return total count for amount of coins owed
    return coins_owed


main()
