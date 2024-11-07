#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
using std::cout; using std::cin; using std::endl;

// Physical base address of FPGA Devices 
const unsigned int LW_BRIDGE_BASE 	= 0xFF200000;  // Base offset 

// Length of memory-mapped IO window 
const unsigned int LW_BRIDGE_SPAN 	= 0x00005000;  // Address map size

// Cyclone V FPGA device addresses
const unsigned int LEDR_BASE 		= 0x00000000;  // Leds offset 
const unsigned int SW_BASE 			= 0x00000040;  // Switches offset
const unsigned int KEY_BASE 		= 0x00000050;  // Push buttons offset
const unsigned int HEX3_HEX0_BASE 	= 0x00000020;  // 7-segment displays offset
const unsigned int HEX5_HEX4_BASE 	= 0x00000030;  // 7-segment displays offset

/** 
 * Write a 4-byte value at the specified general-purpose I/O location. 
 * 
 * @param pBase		Base address returned by 'mmap'. 
 * @parem offset	Offset where device is mapped. 
 * @param value	Value to be written. 
 */ 
void RegisterWrite(char *pBase, unsigned int reg_offset, int value) 
{ 
	* (volatile unsigned int *)(pBase + reg_offset) = value; 
	//volatile prevents the compiler from optimizing code
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
	return * (volatile unsigned int *)(pBase + reg_offset); 
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
	*fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if (*fd == -1)			//  check for errors in openning /dev/mem
	{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
	}
	
   // Get a mapping from physical addresses to virtual addresses
   char *virtual_base = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, *fd, LW_BRIDGE_BASE);
   if (virtual_base == MAP_FAILED)		// check for errors
   {
      cout << "ERROR: mmap() failed..." << endl;
      close (*fd);		// close memory before exiting
      exit(1);        // Returns 1 to the operating system;
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
	if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
	{
      cout << "ERROR: munmap() failed..." << endl;
      exit(1);
	}
   close (fd); 	// close memory
}

int ReadAllSwitches(char *pBase) {
	return RegisterRead(pBase, SW_BASE) & 0x3FF;
}

int Read1Switch(char *pBase, int switchNum) {
	return (ReadAllSwitches(pBase) >> switchNum) & 1;
}

void WriteAllLeds(char *pBase, int value) {
	RegisterWrite(pBase, LEDR_BASE, value & 0x3ff);
}

void Write1Led(char *pBase, int ledNum, bool state) {
	int curLeds = RegisterRead(pBase, LEDR_BASE) & 0x3FF;
	int mask = -1 ^ (1 << ledNum); //1111011111
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
int PushButtonGet(char *pBase) {
	switch (RegisterRead(pBase, KEY_BASE) & 0x3FF) {
        // I have no idea what the actuall memory address is for the push buttons
        // I also have no clue why its 0x3FF but I'm assuming that's the correct value
		case 0x1:
			return 0;
		case 0x2:
			return 1;
		case 0x4:
			return 2;
		case 0x8:
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
void SevenSeg(char *pBase, int value, int displayNum) {
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
    if (value < 0 || value > 9 || displayNum < 0 || displayNum > 3) {
        return;
    }

    // Calculate the offset for the specified display
    unsigned int offset = HEX3_HEX0_BASE + (displayNum * 8);

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
 * @brief Splits a number into its individual digits.
 *
 * This function takes a number from 0 to 1023 and stores its digits in a static array.
 * The positions of the digits correspond to their place values in the number.
 *
 * @param number The integer number to dissect (0 to 1023).
 * @return int The number of digits extracted (1 to 4).
 */
int DigitDissect(int number) {
    // Static array to hold up to four digits
    static char digits[4];

    // Ensure the number is within the valid range
    if (number < 0 || number > 1023) {
        // Initialize digits to zero if the number is invalid
        for (int i = 0; i < 4; i++) {
            digits[i] = 0;
        }
        return 0;
    }

    // Initialize digit count
    int numDigits = 0;

    // Temporary variable to hold the number
    int tempNumber = number;

    // Determine the number of digits
    if (tempNumber == 0) {
        digits[0] = 0;
        numDigits = 1;
    } else {
        // Extract digits from the least significant digit
        while (tempNumber > 0 && numDigits < 4) {
            digits[3 - numDigits] = tempNumber % 10;
            tempNumber /= 10;
            numDigits++;
        }
    }

    // Shift digits to the start of the array
    for (int i = 0; i < numDigits; i++) {
        digits[i] = digits[4 - numDigits + i];
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
	int swIdx = -1;
	cout << "Enter the index of the switch to read (-1 for all): ";
	cin >> swIdx;
	cout << endl;
	int val = 0;
	if (swIdx == -1) {
		val = ReadAllSwitches(pBase);		
		WriteAllLeds(pBase, val);
	} else if (swIdx >= 0 && swIdx <= 9) {
		val = Read1Switch(pBase, swIdx);
		Write1Led(pBase, swIdx, val);
	}


	
	// User Added Functions
	// Secton 4 - Interfacing with Push Buttons
	    // Initialize the counter with the current value of the switches
    int counter = ReadAllSwitches(pBase);
    
    // Display the initial counter value on the LEDs
    WriteAllLeds(pBase, counter);
    
    // Initialize the last button state to -1 (no button pressed)
    int lastButtonState = -1;

     // Variables to hold the digits
    int numDigits = DigitDissect(counter);
    extern char digits[4]; // Declare the external digits array

	    while (true) {
        // Get the current state of the push buttons
        int buttonState = PushButtonGet(pBase);
        
        // Check if the button state has changed since the last check
        if (buttonState != lastButtonState) {
            // Update the last button state to the current state
            lastButtonState = buttonState;
            
            // Perform actions based on which button is pressed
            switch (buttonState) {
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
        WriteAllLeds(pBase, counter);
        // Display the digits on the 7-segment displays
        for (int i = 0; i < numDigits; i++) {
            SevenSeg(pBase, digits[i], i);
        }
    }



	// Done
	Finalize(pBase, fd); 
}
