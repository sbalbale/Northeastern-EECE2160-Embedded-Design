#include <iostream>

using namespace std;

int main() {
    char firstInitial;
    char lastInitial;

    cout << "What's your first initial? ";
    cin >> firstInitial;
    cout << "What's your last initial? ";
    cin >> lastInitial;
    cout << "Hello " << firstInitial << " " << lastInitial << "!" << endl;
    return 0;
}
