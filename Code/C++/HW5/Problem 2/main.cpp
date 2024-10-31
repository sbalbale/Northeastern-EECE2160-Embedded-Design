#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

/**
 * @brief Prints the elements of an array.
 *
 * @param arr The array to be printed.
 * @param size The size of the array.
 */
void PrintArray(const string arr[], int size) {
    cout << "Array: ["; // Print the opening bracket for the array
    for (int i = 0; i < size; i++) { // Loop through each element of the array
        cout << arr[i] << " "; // Print each element followed by a space
    }
    cout << "]" << endl; // Print the closing bracket and end the line
}

/**
 * @brief Fills an array with random strings.
 *
 * @param arr The array to be filled with random strings.
 * @param size The size of the array.
 */
void RandomArray(string arr[], int size) {
    const string charset = "abcdefghijklmnopqrstuvwxyz"; // Define the set of characters to use for generating random strings
    srand(time(nullptr)); // Seed the random number generator with the current time
    for (int i = 0; i < size; i++) {
        string str; // Initialize an empty string to hold the random string
        for (int j = 0; j < 5; j++) {
            str += charset[rand() % charset.size()]; // Append a random character from the charset to the string
        }
        arr[i] = str; // Assign the generated random string to the current array element
    }
}

/**
 * @brief Sorts an array in ascending order using the bubble sort algorithm.
 *
 * @param arr The array to be sorted.
 * @param size The size of the array.
 */

void SortArray(string arr[], int size) {
    for (int i = 0; i < size - 1; i++) { // Loop through each element of the array except the last one
        for (int j = 0; j < size - i - 1; j++) { // Loop through the array up to the unsorted portion
            if (arr[j] > arr[j + 1]) { // If the current element is greater than the next element
                swap(arr[j], arr[j + 1]); // Swap the elements
            }
        }
    }
}

/**
 * @brief Main function to demonstrate the creation, printing, and sorting of an array.
 *
 * Initializes the random number generator with the current time.
 * Creates an array of size 10.
 * Fills the array with random strings and prints it.
 * Sorts the array and prints the sorted array.
 *
 * @return int Exit status of the program.
 */
int main() {
    const int size = 10; // Define the size of the array
    string arr[size]; // Declare an array of strings
    cout << "The Random Array:" << endl; // Print a message indicating the random array will be displayed
    RandomArray(arr, size); // Fill the array with random strings
    PrintArray(arr, size); // Print the random array
    cout << "The Sorted Array:" << endl; // Print a message indicating the sorted array will be displayed
    SortArray(arr, size); // Sort the array
    PrintArray(arr, size); // Print the sorted array
}