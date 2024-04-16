#include <cs50.h>
#include <stdio.h>

bool validateChecksum(long cardNumber);
string checkCardType(long cardNumber);
int get_firstTwoDigits(long cardNumber);
int get_firstDigit(long cardNumber);
int calculateNumberOfDigits(long cardNumber);

int main(void)
{
    // Prompt user for a number and store in long variable
    long cardNumber = get_long("Enter the card number: ");

    bool isValidCard = validateChecksum(cardNumber);
    string cardType = checkCardType(cardNumber);

    if (isValidCard)
    {
        printf("%s\n", cardType);
    }
    else
    {
        printf("INVALID\n");
    }
}

bool validateChecksum(long cardNumber)
{
    // Used to keep track when every other digit has been reached, starting from the end
    int currentDigitPosition = 1;

    int undoubledSum = 0;
    int doubledSum = 0;

    int currentLastDigit = 0;

    while (cardNumber > 0)
    {
        // Get the last digit from the card number
        currentLastDigit = cardNumber % 10;

        // Remove the last digit from the card number
        cardNumber /= 10;

        // If currentDigitPosition is odd, starting from the last digit, the digit will be undoubled
        if (currentDigitPosition % 2 == 1)
        {
            undoubledSum += currentLastDigit;
        }
        // Otherwise, the last digit will be doubled
        else
        {
            currentLastDigit *= 2;

            if (currentLastDigit > 9)
            {
                // Separate the digits into ones and tens, then add them together
                currentLastDigit = (currentLastDigit % 10) + (currentLastDigit / 10);
            }

            doubledSum += currentLastDigit;
        }

        currentDigitPosition++;
    }

    return (((undoubledSum + doubledSum) % 10) == 0);
}

string checkCardType(long cardNumber)
{
    int firstTwoDigits = get_firstTwoDigits(cardNumber);
    int firstDigit = get_firstDigit(cardNumber);
    int numberOfDigits = calculateNumberOfDigits(cardNumber);
    string cardType = "INVALID";

    if (firstDigit == 4 && (numberOfDigits == 13 || numberOfDigits == 16))
    {
        cardType = "VISA";
    }
    else if ((firstDigit == 3 || firstDigit == 5) && (numberOfDigits == 15 || numberOfDigits == 16))
    {
        switch (firstTwoDigits)
        {
            case 34:
            case 37:
                if (numberOfDigits == 15)
                {
                    cardType = "AMEX";
                }
                break;
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                if (numberOfDigits == 16)
                {
                    cardType = "MASTERCARD";
                }
                break;
        }
    }

    return cardType;
}

int get_firstTwoDigits(long cardNumber)
{
    while (cardNumber >= 100)
    {
        cardNumber /= 10;
    }

    return cardNumber;
}

int get_firstDigit(long cardNumber)
{
    while (cardNumber >= 10)
    {
        cardNumber /= 10;
    }

    return cardNumber;
}

int calculateNumberOfDigits(long cardNumber)
{
    int numberOfDigits = 0;

    while (cardNumber > 0)
    {
        cardNumber /= 10;
        numberOfDigits++;
    }

    return numberOfDigits;
}
