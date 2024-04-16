#include <cs50.h>
#include <stdio.h>

int get_cashOwed(void);
int calculateNumberOfCoinsOwed(int cashOwed);

int main(void)
{
    int cashOwed = get_cashOwed();

    int numberOfCoinsOwed = calculateNumberOfCoinsOwed(cashOwed);

    printf("Coins Owed: %i\n", numberOfCoinsOwed);
}

int get_cashOwed(void)
{
    int cashOwed = 0;
    do
    {
        cashOwed = get_int("Enter the amount of cashOwed owed: ");
    }
    while (cashOwed < 0);

    return cashOwed;
}

int calculateNumberOfCoinsOwed(int cashOwed)
{
    // Initialize coin values
    const int QUARTER_VALUE = 25;
    const int DIME_VALUE = 10;
    const int NICKEL_VALUE = 5;
    const int PENNY_VALUE = 1;

    // Initialize coin counters
    int quartersOwed = 0;
    int dimesOwed = 0;
    int nickelsOwed = 0;
    int penniesOwed = 0;

    while (cashOwed > 0)
    {
        if (cashOwed >= QUARTER_VALUE)
        {
            quartersOwed++;
            cashOwed -= QUARTER_VALUE;
        }
        else if (cashOwed >= DIME_VALUE)
        {
            dimesOwed++;
            cashOwed -= DIME_VALUE;
        }
        else if (cashOwed >= NICKEL_VALUE)
        {
            nickelsOwed++;
            cashOwed -= NICKEL_VALUE;
        }
        else if (cashOwed >= PENNY_VALUE)
        {
            penniesOwed++;
            cashOwed -= PENNY_VALUE;
        }
        else
        {
            printf("Something went wrong!\n");
        }
    }

    return quartersOwed + dimesOwed + nickelsOwed + penniesOwed;
}

// int calculateCoins(int numberOfCoinsOwed, int coinValue)
// {
//     numberOfCoinsOwed++;
//     cashOwed -= 1;
// }
