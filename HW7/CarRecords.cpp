#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Car class definition
/**
 * @class Car
 * @brief A class to represent a car record.
 *
 * The Car class encapsulates the details of a car, including its year, color, make, and model.
 * It provides methods to set these fields and retrieve their values.
 *
 * @note The default constructor initializes the car with default values:
 * - year: 0
 * - color: empty string
 * - make: empty string
 * - model: empty string
 *
 * Example usage:
 * @code
 * Car myCar;
 * myCar.setFields(2021, "Red", "Toyota", "Camry");
 * int carYear = myCar.getYear();
 * std::string carColor = myCar.getColor();
 * std::string carMake = myCar.getMake();
 * std::string carModel = myCar.getModel();
 * @endcode
 */
class Car
{
private:
    int year;     // Year of the car
    string color; // Color of the car
    string make;  // Make of the car
    string model; // Model of the car

public:
    // Default constructor
    /**
     * @brief Default constructor for the Car class.
     *
     * Initializes a Car object with default values:
     * - year: 0
     * - color: empty string
     * - make: empty string
     * - model: empty string
     */
    Car() : year(0), color(""), make(""), model("") {}

    // Set fields with specified parameter order
    /**
     * @brief Sets the fields of the car record.
     *
     * @param yr The year of the car.
     * @param cl The color of the car.
     * @param mk The make of the car.
     * @param md The model of the car.
     */
    void setFields(int yr, string cl, string mk, string md)
    {
        year = yr;
        color = cl;
        make = mk;
        model = md;
    }

    // Getter methods
    /**
     * @brief Retrieves the year of the car.
     *
     * @return int The year of the car.
     */
    int getYear() { return year; }
    /**
     * @brief Retrieves the color of the car.
     *
     * @return std::string The color of the car.
     */
    string getColor() { return color; }
    /**
     * @brief Retrieves the make of the car.
     *
     * @return std::string The make of the car.
     */
    string getMake() { return make; }
    /**
     * @brief Retrieves the model of the car.
     *
     * @return std::string The model of the car.
     */
    string getModel() { return model; }
};

// Function to trim leading and trailing whitespace from a string
/**
 * @brief Trims leading and trailing whitespace characters from a given string.
 *
 * This function removes any leading and trailing whitespace characters from the input string.
 * Whitespace characters considered are: space (' '), horizontal tab ('\t'), newline ('\n'),
 * carriage return ('\r'), form feed ('\f'), and vertical tab ('\v').
 *
 * @param str The input string to be trimmed.
 * @return A new string with leading and trailing whitespace removed. If the input string
 *         contains only whitespace characters, an empty string is returned.
 */
string trim(const string &str)
{
    // Find the first non-whitespace character
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    // Find the last non-whitespace character
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    // If the string is all whitespace, return an empty string
    if (first == string::npos || last == string::npos)
        return "";
    // Return the substring with leading and trailing whitespace removed
    return str.substr(first, (last - first + 1));
}

// CarRecords class definition
/**
 * @class CarRecords
 * @brief Manages a collection of car records.
 *
 * The CarRecords class is responsible for reading car records from a file,
 * storing them in a dynamically allocated array, and providing functionalities
 * to print, sort, and identify duplicate records.
 *
 * @details The class provides the following functionalities:
 * - Reading car records from a file and initializing Car objects.
 * - Printing the car records.
 * - Sorting the car records by make (in ascending order) or by year (in descending order).
 * - Identifying and printing duplicate car records.
 *
 * The class ensures that the array size is limited to a maximum of 10 records.
 * If the file cannot be opened, the program will terminate with an error message.
 *
 * @note The CarRecords class assumes the existence of a Car class with appropriate
 * methods for setting and getting car attributes (year, color, make, and model).
 *
 * @file CarRecords.cpp
 */
class CarRecords
{
private:
    int arraySize;   // Keep track of number of records
    ifstream infile; // Input file stream
    Car *cars;       // Pointer to dynamically allocated array of Car objects

public:
    // Constructor reads file records into array
    /**
     * @brief Constructs a CarRecords object with a specified size.
     *
     * This constructor initializes an array of Car objects with a maximum size of 10.
     * It reads car records from the "CarRecords.txt" file and initializes the Car objects
     * with the data from the file.
     *
     * @param size The desired size of the Car array. The actual size will be limited to a maximum of 10.
     *
     * @note If the file "CarRecords.txt" cannot be opened, the program will terminate with an error message.
     *
     * @details The constructor performs the following steps:
     * - Limits the array size to a maximum of 10.
     * - Dynamically allocates an array of Car objects.
     * - Opens the "CarRecords.txt" file for reading.
     * - Reads records from the file and initializes the Car objects with the parsed data.
     * - Adjusts the array size in case there are fewer records than the specified size.
     */
    CarRecords(int size)
    {
        // Limit arraySize to maximum of 10
        arraySize = (size <= 10) ? size : 10;

        // Dynamically allocate array of Car objects
        cars = new Car[arraySize];

        // Open input file
        infile.open("CarRecords.txt");
        if (!infile)
        {
            // Print error message and exit if file cannot be opened
            cerr << "Error opening file CarRecords.txt\n";
            exit(1);
        }

        // Read records from file and initialize Car objects
        string line;
        int count = 0;
        while (getline(infile, line) && count < arraySize)
        {
            // Create a stringstream from the line
            stringstream ss(line);
            string token;
            // Variables to store the parsed data
            string yearStr, color, make, model;
            int year;

            // Read year
            if (getline(ss, token, ','))
            {
                yearStr = trim(token); // Trim whitespace
                year = stoi(yearStr);  // Convert string to integer
            }

            // Read color
            if (getline(ss, token, ','))
            {
                color = trim(token); // Trim whitespace
            }

            // Read make
            if (getline(ss, token, ','))
            {
                make = trim(token); // Trim whitespace
            }

            // Read model
            if (getline(ss, token, ','))
            {
                model = trim(token); // Trim whitespace
            }

            // Set fields in the Car object with correct parameter order
            cars[count].setFields(year, color, make, model);
            count++; // Increment count of records read
        }

        // Adjust arraySize in case there were fewer records
        arraySize = count;
    }

    // Destructor
    /**
     * @brief Destructor for the CarRecords class.
     *
     * This destructor is responsible for cleaning up resources used by the CarRecords class.
     * It performs the following actions:
     * - Frees the dynamically allocated memory for the array of cars.
     * - Closes the input file stream.
     */
    ~CarRecords()
    {
        delete[] cars;  // Free allocated memory
        infile.close(); // Close the file
    }

    // Print car records
    /**
     * @brief Prints the records of all cars.
     *
     * This function prints the details of each car in the cars array, including
     * the year, color, make, and model. The output is formatted with a header
     * and each car's details are printed on a new line.
     */
    void printCarRecords()
    {
        // Print header for car records
        cout << "\nCar Records:\n";
        cout << "------------\n";

        // Iterate through the array of cars and print each car's details
        for (int i = 0; i < arraySize; ++i)
        {
            // Print the year, color, make, and model of the car, separated by commas
            cout << cars[i].getYear() << ", "
                 << cars[i].getColor() << ", "
                 << cars[i].getMake() << ", "
                 << cars[i].getModel() << endl;
        }
    }

    // Sort cars by make (ascending order)
    /**
     * @brief Sorts the array of cars by their make in ascending order.
     *
     * This function uses the standard library sort algorithm to sort the
     * array of Car objects based on the make of the car. The comparison
     * is done using a lambda function that compares the make of two Car
     * objects.
     */
    void sort_cars_by_make()
    {
        // Use std::sort with a lambda function to sort cars by make
        sort(cars, cars + arraySize, [](Car &a, Car &b)
             {
                 // Compare the make of two Car objects
                 return a.getMake() < b.getMake(); });
    }

    // Sort cars by year (descending order)
    /**
     * @brief Sorts the array of cars in descending order by their year.
     *
     * This function uses the standard sort algorithm with a custom comparator
     * to sort the cars array in place. The comparator compares the year of
     * each car and arranges them in descending order.
     */
    void sort_cars_by_year()
    {
        // Use std::sort with a lambda function to sort cars by year in descending order
        sort(cars, cars + arraySize, [](Car &a, Car &b)
             {
                 // Compare the year of two Car objects
                 return a.getYear() > b.getYear(); });
    }

    // Print duplicate records
    /**
     * @brief Prints duplicate car records from the array of cars.
     *
     * This function iterates through the array of car records and compares each
     * car with every other car in the array to find duplicates. A duplicate is
     * defined as two cars having the same year, color, make, and model. If any
     * duplicates are found, they are printed to the console. If no duplicates
     * are found, a message indicating that no duplicate records were found is
     * printed.
     */
    void print_duplicates()
    {
        // Print header for duplicate records
        cout << "\nDuplicate Records:\n";
        cout << "-----------------\n";
        bool found = false; // Flag to indicate if any duplicates are found

        // Iterate through the array of cars
        for (int i = 0; i < arraySize - 1; ++i)
        {
            // Compare each car with every other car in the array
            for (int j = i + 1; j < arraySize; ++j)
            {
                // Check if all fields (year, color, make, model) are the same
                if (cars[i].getYear() == cars[j].getYear() &&
                    cars[i].getColor() == cars[j].getColor() &&
                    cars[i].getMake() == cars[j].getMake() &&
                    cars[i].getModel() == cars[j].getModel())
                {
                    // Print the duplicate car record
                    cout << cars[i].getYear() << ", "
                         << cars[i].getColor() << ", "
                         << cars[i].getMake() << ", "
                         << cars[i].getModel() << endl;
                    found = true; // Set flag to true indicating a duplicate was found
                }
            }
        }

        // If no duplicates were found, print a message indicating so
        if (!found)
        {
            cout << "No duplicate records found.\n";
        }
    }
};

// getInt function outside any class
/**
 * @brief Reads an integer from standard input.
 *
 * This function continuously prompts the user to enter a valid integer.
 * It reads input from the standard input stream (cin) and checks if the input
 * is a valid integer. If the input is valid, it returns the integer.
 * If the input is invalid, it clears the error state of cin, ignores the rest
 * of the input, and prompts the user to enter a valid integer again.
 *
 * @return int The valid integer entered by the user.
 */
int getInt()
{
    int num; // Variable to store the input number
    while (true)
    {
        // Read input from standard input
        if (cin >> num)
        {
            // Check if the next character is a newline or space
            if (cin.peek() == '\n' || cin.peek() == ' ')
            {
                return num; // Return the valid integer
            }
        }
        // Clear the error state of cin
        cin.clear();
        // Ignore the rest of the input until a newline is found
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Prompt the user to enter a valid integer
        cout << "Please enter a valid integer: ";
    }
}

/**
 * @brief Main function to handle car records.
 *
 * This function performs the following tasks:
 * 1. Prompts the user to enter the number of car records to read.
 * 2. Creates a CarRecords object with the specified number of records.
 * 3. Prints the car records.
 * 4. Sorts the car records by year and prints them.
 * 5. Sorts the car records by make and prints them.
 * 6. Checks for duplicate car records and prints them.
 * 7. Deletes the CarRecords object to free allocated memory.
 *
 * @return int Returns 0 upon successful execution.
 */
int main()
{
    // Ask the user for the number of records to read
    cout << "Number of Records to read? ";
    int numRecs = getInt(); // getInt() returns an integer
    CarRecords *cr = new CarRecords(numRecs);
    cout << "\nOriginal Records:\n";
    // Print car records
    cr->printCarRecords();
    // Sort by Year
    cr->sort_cars_by_year();
    cout << "\nSorted by Year:\n";
    // Print car records
    cr->printCarRecords();
    // Sort by Make
    cr->sort_cars_by_make();
    cout << "\nSorted by Make:\n";
    // Print car records
    cr->printCarRecords();
    // Check for Duplicates
    cr->print_duplicates();
    delete cr;
    return 0;
} // end main