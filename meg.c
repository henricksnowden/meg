#include <stdio.h>
#include <stdlib.h>

// Function to read and display the content of a file
void readFile(const char *filename)
{
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL)
    {                                 // Check if the file was opened successfully
        perror("Error opening file"); // Print an error message if the file cannot be opened
        return;                       // Exit the function
    }

    char ch;
    // Read each character from the file until the end of the file is reached
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch); // Print each character to the console
    }

    fclose(file); // Close the file after reading
}

int main(int argc, char *argv[])
{
    // Check if the filename is provided as a command-line argument
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]); // Print the usage message
        exit(EXIT_FAILURE);                                 // Exit the program with an error status
    }

    readFile(argv[1]); // Call the readFile function with the provided filename

    return 0; // Exit the program successfully
}
