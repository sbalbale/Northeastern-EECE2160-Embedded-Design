#include <iostream>
#include <vector>
#include <string>

using namespace std;

double *v; // pointer to the first element in the vector
int size; // size of the vector
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
void listMenu() {
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
void initalize() {
    v = new double[2]; // allocate memory for 2 elements
    ::size = 0; // size of the vector
    capacity = 2; // capacity of the vector
}

void finalize() {
    delete[] v;
}

void grow() {
    cout << "Vector grown" << endl;
    cout << "Previous capacity: " << capacity << endl;
    double *newV = new double[capacity * 2];
    for (int i = 0; i < ::size; i++) {
        newV[i] = v[i];
    }
    delete[] v;
    v = newV;
    capacity *= 2;
    cout << "New capacity: " << capacity << endl;

}

void printVector() {
    cout << "Vector contents:" << endl;
    cout << "Size: " << ::size << endl;
    for (int i = 0; i < ::size; i++) {
        cout << i << ": " << v[i] << endl;
    }
}

void addElement(double element) {
    if (::size == capacity) {
        grow();
    }
    v[::size] = element;
    ::size++;
}

int main()
{
    // vector<string> v;
    initalize();

    while (true) {

        int selection;

        listMenu();
        cin >> selection;

        cout << "You selected: " << selection << endl;

        switch(selection) {
            case 1:
                cout << "List vector's contents" << endl;
                printVector();
                
                break;
            case 2:
                cout << "Append element at the end" << endl;
                cout << "Enter the element to append: ";
                double element;
                cin >> element;
                addElement(element);

                break;
            case 3:
                cout << "Remove last element" << endl;
                // removeLastElement(v);
                break;
            case 4:
                cout << "Insert one element" << endl;
                // insertElement(v);
                break;
            case 5:
                cout << "Exit" << endl;
                finalize();
                return 0;
            default:
                cout << "Invalid selection" << endl;
                break;
        }
        
    }

    finalize();
    return 0;
    
}
