
#include <iostream>

using namespace std;
/**
 * @brief Splits a number into its individual digits.
 *
 * This function takes a number from 0 to 1023 and stores its digits in a static array.
 * The positions of the digits correspond to their place values in the number.
 *
 * @param number The integer number to dissect (0 to 1023).
 * @return int The number of digits extracted (1 to 4).
 */
int DigitDissect(int number, int *digits)
{
    // Ensure the number is within the valid range
    if (number < 0 || number > 1023)
    {
        // Initialize digits to zero if the number is invalid
        for (int i = 0; i < 4; i++)
        {
            digits[i] = 0;
        }
        return 0;
    }

    // Initialize digit count
    int numDigits = 0;

    // Temporary variable to hold the number
    int tempNumber = number;

    // Determine the number of digits
    if (tempNumber == 0)
    {
        digits[0] = 0;
        numDigits = 1;
    }
    else
    {
        // Extract digits from the least significant digit
        while (tempNumber > 0 && numDigits < 4)
        {
            digits[numDigits] = tempNumber % 10;
            tempNumber /= 10;
            numDigits++;
        }
    }

    // Fill the remaining digits with zero
    for (int i = numDigits; i < 4; i++)
    {
        digits[i] = 0;
    }

    return numDigits;
}

int main()
{
    int digits[4];
    int j = 0;
    int numDigits = DigitDissect(123, digits);
    cout << digits << endl;
    while (j < 1023)
    {
        cout << "j: " << j << endl;
        numDigits = DigitDissect(j, digits);
        for (int i = 0; i < 4; i++)
        {
            cout << "i: " << i << " numDigits: " << numDigits << " digit[i]: " << digits[i] << endl;
        }
        j = j + (j + 1);
    }
    return 0;
}