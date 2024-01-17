#include <ctype.h>  // for tolower
#include <stdio.h>  // for printf and scanf
#include <stdlib.h>
#include <string.h>     // strlen function
#include <stdarg.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};    // point system for letters
int compute_score(char word[50]);   // custom function with return type int and argument type char
void clear_input_buffer();          // taakey /n masla na karey

int scrabble(int POINTS[])
{
    system("clear");        // clears the screen saaf suthra kardega
    // Get input words from both players
    char word1[50];         // assuming inputted word would not be more than 50
    char word2[50];
    printYellow("  Welcome to Score More!\n\n\n");
    printYellow("The rules are simple\n");
    printYellow("Score more than your opponent\n");
    sleep(3);           // wait for 3 seconds
    int redo = 0;       // if player wants to play the game again
    do                  // so it works at least once
    {
        system("clear");
        printYellow("\tEvery letter has a specific point value\n\n");
        char alpha = 'A';
        
        for(int i = 0; i < 6; i++)      // for printing the alphabets and their points to the user
        {
            int n = 0;
            printYellow("%c = %i\t%c = %i\t%c = %i\t%c = %i\n", alpha, POINTS[n], alpha + 1, POINTS[n+1], alpha + 2, POINTS[n+2], alpha + 3, POINTS[n+3]);
            alpha = alpha + 4;
            n = n + 4;
        }
        printYellow("%c = %i\t%c = %i\n\n", alpha, POINTS[24], alpha + 1, POINTS[25]);    
        printYellow("Player 1: ");
        clear_input_buffer();       // so it doesnt skip to player 2 directly and clears the buffer of any enter keys or anything
        fgets(word1, sizeof(word1), stdin);     // gets does not work in latest c version and it is unsafe to use apparently
        printYellow("Player 2: ");
        fgets(word2, sizeof(word2), stdin);

        // Score both words
        int score1 = compute_score(word1);
        int score2 = compute_score(word2);
        
        printf("\n");
        printYellow("Player 1's score is %i\n", score1);
        printYellow("Player 2's score is %i\n\n", score2);


        if( score1 > score2 )
        {
            printGreen("Player 1 wins!\n");
        }
        else if( score2 > score1)
        {
            printGreen("Player 2 wins!\n");
        }
        else
        {
            printBlue("Tie!\n");
        }
        printYellow("\n\nEnter 1 if you want to play another game, else enter 0: ");
        scanf("%i", &redo);
        while (redo != 1 && redo != 0)
        {
            printYellow("\n\nWrong number entered. Please enter from the given choice: ");
            scanf(" %i", &redo);
        }
    }
    while (redo == 1);

    printf("\n\n\n");
    printYellow("Thanks for playing!\n");
    return 0;
}

int compute_score(char word[50])
{
    int n = 0, score = 0;
    char word_l;

    for (int i = 0 ; i < strlen(word) ; i++)        // strlen gives the length back of the argument provided
    {
        word_l = tolower(word[i]);                  // tolower converts the alphabet to lowercase   // word_l = h
        if (word_l >= 'a' && word_l <= 'z')
        {
            n = word_l - 'a';                       // taakey hum us letter ka corresponding element nikal sken point ke array me
            score = score + POINTS[n];
        }
        else
        {
            score = score + 0;
        }
    }
    return score;
}
void clear_input_buffer()           // gpt se uthaya
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}