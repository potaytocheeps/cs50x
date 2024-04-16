#include <cs50.h>
#include <stdio.h>

bool is_triangle(float a, float b, float c);

int main(void)
{
    // Make a triangle function.
    // A triangle cannot have a negative length
    // The sum of two of the triangles sides must be greater than the length of the remaining side
    int a = get_int("Side A: ");
    int b = get_int("Side B: ");
    int c = get_int("Side C: ");

    if (is_triangle(a, b, c))
    {
        printf("It is a triangle!\n");
    }
    else
    {
        printf("It is not a triangle!\n");
    }
}

bool is_triangle(float a, float b, float c)
{
    if (a <= 0 || b <= 0 || c <= 0)
    {
        return false;
    }
    if ((a + b > c) && (a + c > b) && (b + c > a))
    {
        return true;
    }
    else
    {
        return false;
    }
}
