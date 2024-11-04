#include <stdio.h>

int main()
{
    char initials[3]; // Assuming initials are two characters plus the null terminator
    printf("Enter your initials: ");
    scanf("%2s", initials); // Read up to 2 characters
    printf("Hello %s\n", initials);
    return 0;
}
