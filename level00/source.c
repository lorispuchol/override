#include <stdio.h>
#include <stdlib.h>

#define CORRECT_PASSWORD 0x149c  // Hexadecimal representation of 5276

int main(void) {
    int input_password;

    // Print welcome message
    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");

    // Prompt for password
    printf("Password: ");
    scanf("%d", &input_password);

    // Check if the input matches the correct password
    if (input_password != CORRECT_PASSWORD) {
        puts("\nInvalid Password!");
        return 1;  // Return non-zero to indicate failure
    } else {
        puts("\nAuthenticated!");
        system("/bin/sh");  // Launch a shell if password is correct
        return 0;  // Return zero to indicate success
    }
}