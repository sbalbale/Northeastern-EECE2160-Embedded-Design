/**
 * 
 * main.cpp
 * 
 * Generates an array of 10 random integer numbers between 0 and 99, 
 * prints the original array, sorts the array in ascending order, 
 * and prints the sorted array.
 * 
 */


#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

/**
 * @brief Prints the elements of an array.
 *
 * @param arr The array to be printed.
 * @param size The size of the array.
 */
// Prints the elements of an array.
void PrintArray(int arr[], int size){
    // Print the opening bracket for the array
    cout << "Array: [";
    // Iterate over each element in the array
    for (int i = 0; i < size; i++){
        // Print the current element followed by a space
        cout << arr[i] << " ";
    }
    // Print the closing bracket for the array and a newline
    cout << "]" << endl;
}

/**
 * @brief Fills an array with random integers between 0 and 99.
 *
 * @param arr The array to be filled with random numbers.
 * @param size The size of the array.
 * @return int* Pointer to the array filled with random numbers.
 */
// Fills an array with random integers between 0 and 99.
int* RandomArray(int* arr, int size){
    // Iterate over each element in the array
    for (int i = 0; i < size; i++){
        // Assign a random integer between 0 and 99 to the current element
        arr[i] = rand() % 100;
    }
    // Return the array filled with random numbers
    return arr;
}

/**
 * @brief Sorts an array in ascending order using the bubble sort algorithm.
 *
 * @param arr The array to be sorted.
 * @param size The size of the array.
 * @return int* Pointer to the sorted array.
 */
int* SortArray(int* arr, int size){
    // Iterate over each element in the array except the last one
    for (int i = 0; i < size - 1; i++){
        // Iterate over the array up to the unsorted portion
        for (int j = 0; j < size - i - 1; j++){
            // If the current element is greater than the next element, swap them
            if (arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
            }
        }
    }
    // Return the sorted array
    return arr;
}

/**
 * @brief Main function to demonstrate the creation, printing, and sorting of an array.
 *
 * Initializes the random number generator with the current time.
 * Creates an array of size 10.
 * Fills the array with random numbers and prints it.
 * Sorts the array and prints the sorted array.
 *
 * @return int Exit status of the program.
 */
int main() {
    srand(time(nullptr)); // Seed the random number generator with the current time
    int size = 10; // Define the size of the array
    int arr[size]; // Declare an array of integers with the specified size
    cout << "The Random Array:" << endl; // Print a message indicating the random array will be displayed
    RandomArray(arr, size); // Fill the array with random numbers
    PrintArray(arr, size); // Print the random array
    cout << "The Sorted Array:" << endl; // Print a message indicating the sorted array will be displayed
    SortArray(arr, size); // Sort the array using bubble sort
    PrintArray(arr, size); // Print the sorted array
}