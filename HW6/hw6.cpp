#include <iostream>
#include <vector>
#include <string>

using namespace std;

double *v;    // pointer to the first element in the vector
int size;     // size of the vector
int capacity; // capacity of the vector

/**
 * @brief Displays the main menu options to the user.
 *
 * This function prints the main menu options to the console, allowing the user to:
 * 1. List the contents of the vector.
 * 2. Append an element at the end of the vector.
 * 3. Remove the last element of the vector.
 * 4. Insert one element into the vector.
 * 5. Exit the program.
 *
 * The user is prompted to select an option from the menu.
 */
void listMenu()
{
    cout << endl;
    cout << "Main menu:" << endl;
    cout << "\t1. List vector's contents" << endl;
    cout << "\t2. Append element at the end" << endl;
    cout << "\t3. Remove last element" << endl;
    cout << "\t4. Insert one element" << endl;
    cout << "\t5. Exit" << endl;
    cout << "Select an option: ";
}

/**
 * @brief Initializes the vector by allocating memory for 2 elements.
 *
 * This function sets up the initial state of the vector by:
 * - Allocating memory for 2 double elements.
 * - Setting the initial size of the vector to 0.
 * - Setting the initial capacity of the vector to 2.
 */
void initialize()
{
    v = new double[2]; // allocate memory for 2 elements
    ::size = 0;        // size of the vector
    capacity = 2;      // capacity of the vector
}

/**
 * @brief Finalizes the program by deallocating the memory allocated for the array 'v'.
 *
 * This function should be called to clean up resources before the program exits
 * to prevent memory leaks.
 */
void finalize()
{
    // Deallocate the memory allocated for the array 'v' to prevent memory leaks
    delete[] v;
}

/**
 * @brief Grows the capacity of the vector.
 *
 * This function doubles the current capacity of the vector, allocates a new array with the increased capacity,
 * copies the existing elements to the new array, deletes the old array, and updates the capacity.
 * It also prints the previous and new capacities to the standard output.
 */
void grow()
{
    // Print a message indicating that the vector is being grown
    cout << "Vector grown" << endl;

    // Print the previous capacity of the vector
    cout << "Previous capacity: " << capacity << endl;

    // Allocate a new array with double the current capacity
    double *newV = new double[capacity * 2];

    // Copy the existing elements to the new array
    for (int i = 0; i < ::size; i++)
    {
        newV[i] = v[i];
    }

    // Deallocate the memory of the old array
    delete[] v;

    // Update the pointer to point to the new array
    v = newV;

    // Double the capacity
    capacity *= 2;

    // Print the new capacity of the vector
    cout << "New capacity: " << capacity << endl;
}

/**
 * @brief Prints the contents of a global vector.
 *
 * This function prints the contents of a global vector `v` to the standard output.
 * It first prints a header "Vector contents:" followed by the size of the vector.
 * Then, it iterates through the vector and prints each element with its index.
 *
 * Note: This function assumes the existence of a global vector `v` and a global
 * integer `size` representing the size of the vector.
 */
void printVector()
{
    // Print the header for the vector contents
    cout << "Vector contents:" << endl;

    // Print the size of the vector
    cout << "Size: " << ::size << endl;

    // Iterate through the vector and print each element with its index
    for (int i = 0; i < ::size; i++)
    {
        cout << i << ": " << v[i] << endl;
    }
}

/**
 * @brief Adds a new element to the dynamic array.
 *
 * Prompts the user to enter an element, checks if the current size has reached the capacity,
 * and grows the array if necessary. Then, it adds the new element to the array and increments
 * the size of the array.
 */
void addElement()
{
    // Prompt the user to enter the element to append
    cout << "Enter the element to append: ";

    // Declare a variable to store the new element
    double element;

    // Read the element from the user input
    cin >> element;

    // Check if the current size has reached the capacity
    if (::size == capacity)
    {
        // Grow the array if the capacity is reached
        grow();
    }

    // Add the new element to the array at the current size index
    v[::size] = element;

    // Increment the size of the array
    ::size++;
}

/**
 * @brief Removes the last element from the vector.
 *
 * This function decreases the size of the vector by one. If the vector is
 * already empty, it prints a message indicating that the vector is empty
 * and does not perform any removal.
 */
void removeElement()
{
    // Check if the vector is already empty
    if (::size == 0)
    {
        // Print a message indicating that the vector is empty
        cout << "Vector is empty" << endl;
        // Exit the function without performing any removal
        return;
    }
    // Decrease the size of the vector by one
    ::size--;
}

/**
 * @brief Inserts an element into the vector at the specified index.
 *
 * Prompts the user to enter the index at which to insert the element and the element itself.
 * If the index is invalid (less than 0 or greater than the current size of the vector),
 * an error message is displayed and the function returns without making any changes.
 * If the vector is at full capacity, it is grown to accommodate the new element.
 * The elements from the specified index onwards are shifted to the right to make space for the new element.
 * The new element is then inserted at the specified index, and the size of the vector is incremented.
 *
 * @note Assumes that the vector `v`, its current size `::size`, and its capacity `capacity`
 * are defined globally. Also assumes that the function `grow()` is defined to handle vector resizing.
 */
void insertElement()
{
    // Prompt the user to enter the index at which to insert the element
    cout << "Enter the index to insert: ";
    int index;
    cin >> index;

    // Prompt the user to enter the element to insert
    cout << "Enter the element to insert: ";
    int element;
    cin >> element;

    // Check if the index is valid (within the range [0, ::size])
    if (index < 0 || index > ::size)
    {
        // Print an error message if the index is invalid
        cout << "Invalid index" << endl;
        // Return without making any changes
        return;
    }

    // Check if the vector is at full capacity
    if (::size == capacity)
    {
        // Print a message indicating that the vector is being grown
        cout << "Vector grown" << endl;
        // Grow the vector to accommodate the new element
        grow();
    }

    // Shift elements from the specified index onwards to the right
    for (int i = ::size; i > index; i--)
    {
        v[i] = v[i - 1];
    }

    // Insert the new element at the specified index
    v[index] = element;

    // Print a message indicating that the element has been inserted
    cout << "Inserted element " << element << " at index " << index << endl;

    // Increment the size of the vector
    ::size++;
}

/**
 * @brief Entry point of the program.
 *
 * This function initializes the program, displays a menu to the user, and performs actions based on the user's selection.
 * The menu options include:
 * 1. List vector's contents
 * 2. Append element at the end
 * 3. Remove last element
 * 4. Insert one element
 * 5. Exit the program
 *
 * The program runs in an infinite loop until the user selects the exit option.
 *
 * @return int Returns 0 upon successful completion.
 */
int main()
{
    // Initialize the vector and other necessary components
    initialize();

    // Run an infinite loop to display the menu and perform actions based on user input
    while (true)
    {

        // Variable to store the user's menu selection
        int selection;

        // Display the menu to the user
        listMenu();

        // Read the user's menu selection
        cin >> selection;

        // Print the user's selection
        cout << "You selected: " << selection << endl;

        // Perform actions based on the user's selection
        switch (selection)
        {
        case 1:
            // Option 1: List vector's contents
            cout << "List vector's contents" << endl;
            printVector();
            break;
        case 2:
            // Option 2: Append element at the end
            cout << "Append element at the end" << endl;
            addElement();
            break;
        case 3:
            // Option 3: Remove last element
            cout << "Remove last element" << endl;
            removeElement();
            break;
        case 4:
            // Option 4: Insert one element
            cout << "Insert one element" << endl;
            insertElement();
            break;
        case 5:
            // Option 5: Exit the program
            cout << "Exit" << endl;
            finalize();
            return 0;
        default:
            // Handle invalid menu selections
            cout << "Invalid selection" << endl;
            break;
        }
    }

    // Finalize the program (cleanup resources)
    finalize();
    return 0;
}