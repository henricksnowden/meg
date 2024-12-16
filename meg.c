/*
*Copyright (C) 2018 Trasicio Maina <cisiomaina@gmail.com>

*This program is free software; you can redistribute it and/or modify it under the terms of the GNU
*General Public License version 3 as published by the Free Software Foundation.

*This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
*without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*See the GNU General Public License for more details.

*You should have received a copy of the GNU General Public License along with this program; if not,
*write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
*/

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

// Function to display version information
void displayVersion()
{
    printf("The Meg version 0.01\n");
    printf("Written by Trasicio Maina.\n");
}

// Function to display help information and usage examples
void displayHelp()
{
    displayVersion();
    printf("\nUsage: text_editor <filename>\n");
    printf("Read and display the content of a file.\n");
    printf("\nOptions:\n");
    printf(" --help\tDisplay this help message and exit\n");
    printf(" --version\tDisplay version information and exit\n");
    printf("\nExamples:\n");
    printf(" text_editor file.txt\tRead and display the content of file.txt\n");
    printf(" text_editor --help\tDisplay this help message\n");
    printf(" text_editor --version\tDisplay version information\n");
}

int main(int argc, char *argv[])
{
    // Check if the filename is provided as a command-line argument
    if (argc < 2) // Point of note: the program name counts as one argument :)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]); // Print the usage message
        exit(EXIT_FAILURE);                                 // Exit the program with an error status
    }

    readFile(argv[1]); // Call the readFile function with the provided filename

    return 0; // Exit the program successfully
}
