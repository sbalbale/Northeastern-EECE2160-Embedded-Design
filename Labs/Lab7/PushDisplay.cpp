#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

// Physical base address of FPGA Devices
const unsigned int LW_BRIDGE_BASE = 0xFF200000; // Base offset

// Length of memory-mapped IO window
const unsigned int LW_BRIDGE_SPAN = 0x00005000; // Address map size

// Cyclone V FPGA device addresses
const unsigned int LEDR_BASE = 0x00000000;      // Leds offset
const unsigned int SW_BASE = 0x00000040;        // Switches offset
const unsigned int KEY_BASE = 0x00000050;       // Push buttons offset
const unsigned int HEX3_HEX0_BASE = 0x00000020; // 7-segment displays offset
const unsigned int HEX5_HEX4_BASE = 0x00000030; // 7-segment displays offset

/**
 * Write a 4-byte value at the specified general-purpose I/O location.
 *
 * @param pBase		Base address returned by 'mmap'.
 * @parem offset	Offset where device is mapped.
 * @param value	Value to be written.
 */
void RegisterWrite(char *pBase, unsigned int reg_offset, int value)
{
    *(volatile unsigned int *)(pBase + reg_offset) = value;
    // volatile prevents the compiler from optimizing code
}

/**
 * Read a 4-byte value from the specified general-purpose I/O location.
 *
 * @param pBase		Base address returned by 'mmap'.
 * @param offset	Offset where device is mapped.
 * @return		Value read.
 */
int RegisterRead(char *pBase, unsigned int reg_offset)
{
    return *(volatile unsigned int *)(pBase + reg_offset);
}

/**
 * Initialize general-purpose I/O
 *  - Opens access to physical memory /dev/mem
 *  - Maps memory into virtual address space
 *
 * @param fd	File descriptor passed by reference, where the result
 *			of function 'open' will be stored.
 * @return	Address to virtual memory which is mapped to physical, or MAP_FAILED on error.
 */
char *Initialize(int *fd)
{
    // Open /dev/mem to give access to physical addresses
    *fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (*fd == -1) //  check for errors in openning /dev/mem
    {
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
    }

    // Get a mapping from physical addresses to virtual addresses
    char *virtual_base = (char *)mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, *fd, LW_BRIDGE_BASE);
    if (virtual_base == MAP_FAILED) // check for errors
    {
        cout << "ERROR: mmap() failed..." << endl;
        close(*fd); // close memory before exiting
        exit(1);    // Returns 1 to the operating system;
    }
    return virtual_base;
}

/**
 * Close general-purpose I/O.
 *
 * @param pBase	Virtual address where I/O was mapped.
 * @param fd	File descriptor previously returned by 'open'.
 */
void Finalize(char *pBase, int fd)
{
    if (munmap(pBase, LW_BRIDGE_SPAN) != 0)
    {
        cout << "ERROR: munmap() failed..." << endl;
        exit(1);
    }
    close(fd); // close memory
}

int ReadAllSwitches(char *pBase)
{
    return RegisterRead(pBase, SW_BASE) & 0x3FF;
}

int Read1Switch(char *pBase, int switchNum)
{
    return (ReadAllSwitches(pBase) >> switchNum) & 1;
}

void WriteAllLeds(char *pBase, int value)
{
    RegisterWrite(pBase, LEDR_BASE, value & 0x3ff);
}

void Write1Led(char *pBase, int ledNum, bool state)
{
    int curLeds = RegisterRead(pBase, LEDR_BASE) & 0x3FF;
    int mask = -1 ^ (1 << ledNum); // 1111011111
    curLeds = (curLeds & mask) | (state << ledNum);
    WriteAllLeds(pBase, curLeds);
}

// User Added Functions
// Secton 4 - Interfacing with Push Buttons

/**
 * @brief Reads the state of push buttons and returns the corresponding button index.
 *
 * This function reads the value from the push button register and determines which button
 * is pressed based on the bit pattern. It returns the index of the pressed button or -1
 * if no button is pressed.
 *
 * @param pBase Pointer to the base address of the push button register.
 * @return int Index of the pressed button (0, 1, 2, or 3), or -1 if no button is pressed.
 */
int PushButtonGet(char *pBase)
{
    // cout << RegisterRead(pBase, KEY_BASE) << endl;
    switch (RegisterRead(pBase, KEY_BASE))
    {
    case 0:
        return -2;
    case 1:
        return 0;
    case 2:
        return 1;
    case 4:
        return 2;
    case 8:
        return 3;
    default:
        return -1;
    }
}

/**
 * @brief Displays a given value on a specified 7-segment display.
 *
 * This function takes a base address, a value, and a place index to display the value
 * on a 7-segment display. The value should be a single digit (0-9) and the place index
 * specifies which 7-segment display to use.
 *
 * @param pBase Pointer to the base address of the 7-segment display.
 * @param value The digit to be displayed (0-9).
 * @param displayNum The index of the 7-segment display to use (0-3).
 */
void SevenSeg(char *pBase, int value, int displayNum)
{
    // 7-segment display encodings for digits 0-9
    unsigned char sevenSegDigits[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    // Check for valid input
    if (value < 0 || value > 9 || displayNum < 0 || displayNum > 3)
    {
        return;
    }

    // Calculate the offset for the specified display
    unsigned int offset = HEX3_HEX0_BASE + (displayNum);

    // Write the encoding to the specified 7-segment display
    RegisterWrite(pBase, offset, sevenSegDigits[value]);

    // Display the value on the 7-segment display
    // if (value >= 0 && value <= 9) {
    //     RegisterWrite(pBase, HEX3_HEX0_BASE, sevenSegValues[value]);
    // }
    // else if (value > 9 && value <= 99) {
    //     int tens = value / 10;
    //     int ones = value % 10;
    //     RegisterWrite(pBase, HEX3_HEX0_BASE, (sevenSegValues[ones] << 8) | sevenSegValues[tens]);
    // }
    // else if (value > 99 && value <= 999) {
    //     int hundreds = value / 100;
    //     int tens = (value / 10) % 10;
    //     int ones = value % 10;
    //     RegisterWrite(pBase, HEX3_HEX0_BASE, (sevenSegValues[ones] << 16) | (sevenSegValues[tens] << 8) | sevenSegValues[hundreds]);
    // }
}

/**
 * @brief Dissects a given number into its individual digits and stores them in an array.
 *
 * This function takes an integer number and dissects it into its individual digits,
 * storing each digit in the provided array. The digits are stored in reverse order,
 * with the least significant digit at index 0. If the number is invalid (less than 0
 * or greater than 1023), the array is initialized to zero and the function returns 0.
 *
 * @param number The integer number to be dissected. Must be between 0 and 1023 inclusive.
 * @param digits Pointer to an array of integers where the dissected digits will be stored.
 *               The array must have space for at least 4 integers.
 * @return The number of digits in the provided number.
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

/**
 * @brief Main function to initialize hardware, read switch states, and control LEDs.
 *
 * The program initializes the hardware, reads the state of switches, and controls the LEDs based on the switch states.
 * It also includes a loop to interface with push buttons to modify a counter and update the LEDs accordingly.
 *
 * @return int Exit status of the program.
 *
 * The main function performs the following steps:
 * 1. Initializes the hardware and gets the base address for memory-mapped I/O.
 * 2. Prompts the user to enter the index of the switch to read.
 * 3. Reads the state of the specified switch or all switches.
 * 4. Writes the switch state(s) to the corresponding LED(s).
 * 5. Initializes a counter with the state of all switches.
 * 6. Enters an infinite loop to read the state of push buttons and modify the counter based on the button state:
 *    - Button 0: Increment the counter by 1.
 *    - Button 1: Decrement the counter by 1.
 *    - Button 2: Shift the counter right by 1.
 *    - Button 3: Shift the counter left by 1.
 *    - Button -1: Set the counter to the value of all switches.
 * 7. Updates the LEDs with the current counter value.
 * 8. Finalizes the hardware before exiting.
 */
int main()
{
    // Initialize
    int fd;
    char *pBase = Initialize(&fd);

    // User Added Functions
    // Secton 4 - Interfacing with Push Buttons
    int counter = ReadAllSwitches(pBase);

    // Display the initial counter value on the LEDs
    WriteAllLeds(pBase, counter);

    // Initialize the last button state to -1 (no button pressed)
    int lastButtonState = -1;

    // Variables to hold the digits
    int digits[4];
    int numDigits = DigitDissect(counter, digits);

    while (true)
    {
        cout << "lastButtonState: " << lastButtonState << " counter: " << counter << endl;
        // Get the current state of the push buttons
        int buttonState = PushButtonGet(pBase);

        // Check if the button state has changed since the last check
        if (buttonState != lastButtonState)
        {
            // Update the last button state to the current state
            lastButtonState = buttonState;

            // Perform actions based on which button is pressed
            switch (buttonState)
            {
            case 0:
                // Increment the counter by 1 when KEY0 is pressed
                counter++;
                break;
            case 1:
                // Decrement the counter by 1 when KEY1 is pressed
                counter--;
                break;
            case 2:
                // Shift the counter right by one bit when KEY2 is pressed
                counter = counter >> 1;
                break;
            case 3:
                // Shift the counter left by one bit when KEY3 is pressed
                counter = counter << 1;
                break;
            case -1:
                // Set the counter to the value of the switches when no button is pressed
                counter = ReadAllSwitches(pBase);
                break;
            }
        }

        if (counter > 1023)
        {
            counter = 0;
        }
        else if (counter < 0)
        {
            counter = 1023;
        }

        WriteAllLeds(pBase, counter);
        // Display the digits on the 7-segment displays
        numDigits = DigitDissect(counter, digits);

        for (int i = 0; i < numDigits; i++)
        {
            cout << "i: " << i << " numDigits: " << numDigits << " digit[i]: " << digits[i] << endl;
            SevenSeg(pBase, digits[i], i);
        }
    }

    // Done
    Finalize(pBase, fd);
}
