#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>     // So i can interrupt with control + c or else music loop blocks it
#include <SDL2/SDL.h>       // for bg music
#include <SDL2/SDL_mixer.h>     // for bg music
// gcc -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer -o tictactoe tictactoe.c

void guide();
int run_again(int *play_again);     // so user can play again
void print_table(char table[3][3]); // to print the tictactoe table
void checkrow(char table[3][3], char play1, char play2, int *win);      //
void checkcol(char table[3][3], char play1, char play2, int *win);
void clear_input_buffer();           // gpt se uthaya

int tictactoe(void)
{
    int play_again;
    system("clear");
    printYellow("\nWelcome to the game of tictactoe!\nThis is a 2 player game\n\n");
    guide();
    do
    {
        play_again = 0;
        int win = 0;
        int x, y;
        char play1, play2, table[3][3];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                table[i][j] = ' ';
            }
        }
        print_table(table);
        char symbolInput[100];
        printYellow("Player 1, enter your symbol please: ");
        clear_input_buffer();
        fgets(symbolInput, 100, stdin);
        while(strlen(symbolInput) != 2 || symbolInput[0] == '\n')
        {
            printYellow("Please enter a single character: ");
            fgets(symbolInput, 100, stdin);
        }
        play1 = symbolInput[0];
        
        printYellow("Player 2, enter your symbol please: ");
        fgets(symbolInput, 100, stdin);
        while(strlen(symbolInput) != 2 || symbolInput[0] == '\n' || symbolInput[0] == play1)
        {
            printYellow("Please enter a different single character: ");
            fgets(symbolInput, 100, stdin);
        }
        play2 = symbolInput[0];
        while (play2 == play1)
        {
            printYellow("Player 2, enter a different symbol please: ");
            scanf(" %c", &play2);
        }
        printYellow("\n");
        printYellow("Enter the coordinate you would like to pick in a 0,0 pattern\n");

        for (int i = 0; i < 9; i++)
        {
            char coordInput[10]; // Array to store coordinate input
            int validInput;
            if (i % 2 == 0)
            {
                printYellow("Player 1's turn. Enter the coordinate you would like to pick: ");
            }
            else
            {
                printYellow("Player 2's turn. Enter the coordinate you would like to pick: ");
            }

            do
            {
                fgets(coordInput, 10, stdin);
                validInput = sscanf(coordInput, "%i,%i", &x, &y);
                if (validInput != 2 || x < 0 || x > 2 || y < 0 || y > 2)
                {
                    printYellow("Invalid input. Enter coordinates in the format x,y (0 <= x,y <= 2): ");
                    validInput = 0;
                }
                else if (table[x][y] == play1 || table[x][y] == play2)
                {
                    printYellow("Spot already taken. Please enter a different coordinate: ");
                    validInput = 0;
                }
            }
            while (!validInput);

            table[x][y] = (i % 2 == 0) ? play1 : play2;

            system("clear"); // This command clears the terminal screen on Unix-based systems like macOS
            print_table(table);
            if (i >= 4)
            {
                // for rows
                checkrow(table, play1, play2, &win);
                // for columns
                checkcol(table, play1, play2, &win);
                // for diagonal
                if ((table[0][0] == table[1][1] && table[1][1] == table[2][2]) ||
                    (table[0][2] == table[1][1] && table[1][1] == table[2][0]))
                {
                    if (table[1][1] == play1)
                    {
                        printGreen("Player 1 is the Winner!\n");
                        win = 1;
                        break;
                    }
                    else if (table[1][1] == play2)
                    {
                        printGreen("Player 2 is the Winner!\n");
                        win = 1;
                        break;
                    }
                }
            }
            if (win == 1)
            {
                break;
            }
        }
        if (win == 0)
        {
            printBlue("Tie!\n");
        }
        run_again(&play_again);     // taakey wo address par jaakar dekhy kia maamlat hain or wahi pass karrey
        if (play_again == 1)
        {
            system("clear");
        }
    }
    while (play_again == 1);

    if (play_again == 0)
    {
        printYellow("Thanks for playing!\n\n\n");
    }
    return 0;
}
int run_again(int *play_again)      // passing by pointer because if i dont, main ke andar wali val affect nhi hogi
{
    printYellow("\nWould you like to play again?\n");
    printYellow("\nEnter 1 to play again, 0 to end: ");
    scanf("%i", play_again);
    return *play_again;
}
void print_table(char table[3][3])
{
    printYellow(" %c | %c | %c \n", table[0][0], table[0][1], table[0][2]);
    printYellow("-----------\n");
    printYellow(" %c | %c | %c \n", table[1][0], table[1][1], table[1][2]);
    printYellow("-----------\n");
    printYellow("  %c | %c | %c \n\n", table[2][0], table[2][1], table[2][2]);
}
void checkrow(char table[3][3], char play1, char play2, int *win)
{
    for (int row = 0; row < 3; row++)
    {
        if (table[row][0] == table[row][1] && table[row][1] == table[row][2] && table[row][0] != ' ')
        {
            if (table[row][0] == play1)
            {
                printGreen("Player 1 is the Winner!\n");
                *win = 1;
                break; 
            }
            else
            {
                printGreen("Player 2 is the Winner!\n");
                *win = 1;
            }
        }
    }
}
void checkcol(char table[3][3], char play1, char play2, int *win)
{
    for (int col = 0; col < 3; col++)
    {
        if (table[0][col] == table[1][col] && table[1][col] == table[2][col] && table[0][col] != ' ')
        {
            if (table[0][col] == play1)
            {
                printGreen("Player 1 is the Winner!\n");
                *win = 1;
                break;
            }
            else
            {
                printGreen("Player 2 is the Winner!\n");
                *win = 1;
            }
        }
    }
}
// Add a guide
void guide()
{
    printYellow("Input must be in the following format row column e.g 0,0\n");
    printYellow("Each index in this game has a coordinate\n\n\n");
	printYellow(" (0,0) | (0,1) | (0,2) \n");
    printYellow("---------------------\n");
    printYellow("  (1,0) | (1,1) | (1,2)  \n");
    printYellow("---------------------\n");
    printYellow("    (2,0) | (2,1) | (2,2) \n\n\n\n");
}

