/*
 * Copyright (C) 2024 Trasicio Maina <cisiomaina@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   // Include errno.h for error handling.
#include <ncurses.h> // Include ncurses for keyboard handling and screen display.

#define CTRL_KEY(k) ((k) & 0x1f) // Macro to define control key combinations.
#define BACKSPACE 127            // Define the backspace key code.

// Function prototypes.
void readFile(const char *filename);
void writeFile(const char *filename, const char *content);
void displayVersion();
void displayHelp();
void textEditor(const char *filename);

// Global variables to manage buffer content.
char *buffer = NULL;
size_t bufferSize = 0;
size_t bufferLength = 0;
int modified = 0; // Flag to track if the buffer has been modified.

// Function to read and display the content of a file.
void readFile(const char *filename)
{
    FILE *file = fopen(filename, "r"); // Open the file in read mode.
    if (file == NULL)
    {                                 // Check if the file was opened successfully.
        perror("Error opening file"); // Print an error message if the file cannot be opened.
        return;                       // Exit the function.
    }

    // Move the file pointer to the end of the file and get the file size.
    fseek(file, 0, SEEK_END);
    bufferSize = ftell(file);
    rewind(file); // Rewind the file pointer to the beginning.

    // Allocate memory for the buffer to store file content.
    buffer = malloc(bufferSize + 1);
    if (buffer == NULL)
    {                                      // Check if memory allocation was successful.
        perror("Error allocating memory"); // Print an error message if memory allocation fails.
        fclose(file);                      // Close the file.
        return;                            // Exit the function.
    }

    // Read the file content into the buffer.
    bufferLength = fread(buffer, 1, bufferSize, file);
    buffer[bufferLength] = '\0'; // Null-terminate the buffer.

    fclose(file); // Close the file.
}

// Function to write buffer content to a file.
void writeFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w"); // Open the file in write mode.
    if (file == NULL)
    {                                             // Check if the file was opened successfully.
        perror("Error opening file for writing"); // Print an error message if the file cannot be opened.
        return;                                   // Exit the function.
    }

    // Write the content to the file.
    fwrite(content, 1, strlen(content), file);
    fclose(file); // Close the file.
}

// Function to display version information.
void displayVersion()
{
    printf("The Meg version 0.01 (c) Trasicio Maina, 2024.\n");
}

// Function to display help information and usage examples.
void displayHelp()
{
    printf("\n");
    displayVersion(); // Display version information.
    printf("\nUsage: meg <filename>\n");
    printf("Read and display the content of a file.\n");
    printf("\nOptions:\n");
    printf("  --help\tDisplay this help message and exit\n");
    printf("  --version\tDisplay version information and exit\n");
    printf("\nWriting a New File:\n");
    printf("  meg newfile.txt\tStart a new file and edit its content\n");
    printf("  Ctrl+S\tSave the file\n");
    printf("  Ctrl+Q\tQuit the program\n");
    printf("\n");
}

// Function to handle text editing, including saving and quitting.
void textEditor(const char *filename)
{
    initscr();            // Initialize the ncurses screen.
    raw();                // Enable raw mode for direct input.
    keypad(stdscr, TRUE); // Enable keyboard mapping.
    noecho();             // Disable echoing of typed characters.

    // Allocate initial buffer memory if not reading from a file.
    if (buffer == NULL)
    {
        bufferSize = 1024; // Initial buffer size.
        buffer = malloc(bufferSize);
        if (buffer == NULL)
        {
            perror("Error allocating initial buffer");
            endwin();
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printw("%s", buffer); // Display the existing buffer content if available.
    }

    int ch;
    // Main loop to process user input.
    while ((ch = getch()) != CTRL_KEY('q'))
    { // Check for Ctrl+Q to quit.
        switch (ch)
        {
        case CTRL_KEY('s'):                        // Check for Ctrl+S to save.
            writeFile(filename, buffer);           // Save the buffer content to the file.
            modified = 0;                          // Reset the modified flag.
            mvprintw(LINES - 1, 0, "File saved."); // Display a save confirmation message.
            refresh();
            break;
        case BACKSPACE: // Handle the backspace key.
        case KEY_BACKSPACE:
            if (bufferLength > 0)
            {
                buffer[--bufferLength] = '\0'; // Remove the last character from the buffer.
                int y, x;
                getyx(stdscr, y, x); // Get the current cursor position.
                mvdelch(y, x - 1);   // Move the cursor back and delete the character.
                modified = 1;        // Set the modified flag.
                refresh();
            }
            break;
        default: // Handle other input characters.
            if (bufferLength + 1 >= bufferSize)
            {
                bufferSize *= 2; // Double the buffer size.
                buffer = realloc(buffer, bufferSize);
                if (buffer == NULL)
                {
                    perror("Error reallocating buffer");
                    endwin();
                    exit(EXIT_FAILURE);
                }
            }
            buffer[bufferLength++] = ch; // Add character to the buffer.
            buffer[bufferLength] = '\0'; // Null-terminate the buffer.
            modified = 1;                // Set the modified flag.
            addch(ch);                   // Add character to the screen.
            refresh();
            break;
        }
    }

    // Prompt to save if there are unsaved changes.
    if (modified)
    {
        mvprintw(LINES - 1, 0, "You have unsaved changes. Save before quitting? (y/n)");
        refresh();
        int saveCh = getch(); // Get user input for saving.
        if (saveCh == 'y')
        {                                // Check if user wants to save.
            writeFile(filename, buffer); // Save the buffer content to the file.
        }
    }

    endwin(); // End ncurses mode.
}

int main(int argc, char *argv[])
{
    // Check if the --help or --version options are provided.
    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            displayHelp();      // Display the help information.
            exit(EXIT_SUCCESS); // Exit the program successfully.
        }
        else if (strcmp(argv[1], "--version") == 0)
        {
            displayVersion();   // Display the version information.
            exit(EXIT_SUCCESS); // Exit the program successfully.
        }
    }

    // Check if the filename is provided as a command-line argument.
    if (argc < 2)
    {                                                       // Point of note: the program name counts as one argument :)
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]); // Print the usage message.
        exit(EXIT_FAILURE);                                 // Exit the program with an error status.
    }

    // Read the file content into the buffer.
    readFile(argv[1]);
    // Start the text editor.
    textEditor(argv[1]);

    // Free the allocated buffer memory.
    free(buffer);
    return 0; // Exit the program successfully.
}
