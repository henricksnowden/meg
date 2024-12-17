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
#include <errno.h>
#include <ncurses.h> // Include ncurses for keyboard handling and screen display

#define CTRL_KEY(k) ((k) & 0x1f)

void readFile(const char *filename);
void writeFile(const char *filename, const char *content);
void displayVersion();
void displayHelp();

char *buffer = NULL;
size_t bufferSize = 0;
size_t bufferLength = 0;
int modified = 0;

void readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    bufferSize = ftell(file);
    rewind(file);

    buffer = malloc(bufferSize + 1);
    if (buffer == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    bufferLength = fread(buffer, 1, bufferSize, file);
    buffer[bufferLength] = '\0';

    fclose(file);
}

void writeFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    fwrite(content, 1, strlen(content), file);
    fclose(file);
}

void displayVersion()
{
    printf("The Meg version 0.01 (c) Trasicio Maina, 2024.\n");
}

void displayHelp()
{
    printf("\n");
    displayVersion();
    printf("\nUsage: text_editor <filename>\n");
    printf("Read and display the content of a file.\n");
    printf("\nOptions:\n");
    printf("  --help\tDisplay this help message and exit\n");
    printf("  --version\tDisplay version information and exit\n");
    printf("\nExamples:\n");
    printf("  meg file.txt\tRead and display the content of file.txt\n");
    printf("  meg --help\tDisplay this help message\n");
    printf("  meg --version\tDisplay version information\n");
    printf("\n");
}

void textEditor(const char *filename)
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    if (buffer != NULL)
    {
        printw("%s", buffer);
    }

    int ch;
    while ((ch = getch()) != CTRL_KEY('q'))
    {
        switch (ch)
        {
        case CTRL_KEY('s'):
            writeFile(filename, buffer);
            modified = 0;
            mvprintw(LINES - 1, 0, "File saved.");
            break;
        default:
            buffer[bufferLength++] = ch;
            buffer[bufferLength] = '\0';
            modified = 1;
            addch(ch);
            break;
        }
    }

    if (modified)
    {
        mvprintw(LINES - 1, 0, "You have unsaved changes. Save before quitting? (y/n)");
        int saveCh = getch();
        if (saveCh == 'y')
        {
            writeFile(filename, buffer);
        }
    }

    endwin();
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            displayHelp();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[1], "--version") == 0)
        {
            displayVersion();
            exit(EXIT_SUCCESS);
        }
    }

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFile(argv[1]);
    textEditor(argv[1]);

    free(buffer);
    return 0;
}
