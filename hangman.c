#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <AudioToolbox/AudioToolbox.h>
// gcc -o hangman hangman.c -framework AudioToolbox -framework CoreFoundation

// each of the text files contains 1000 words
#define LISTSIZE 1000
#define TERMINAL_WIDTH 120
#define BRIGHT_GREEN "\033[38;5;46m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define RESET_COLOR "\033[0m"
void printRed(const char *format, ...);
void printGreen(const char *format, ...);
void printBlue(const char *format, ...);
void printYellow(const char *format, ...);
void printYellowWoSpaces(const char *format, ...);
void audio_shorten(const char *file_path);
void play_correct_audio(void);
void play_ghalat_hai_boss_audio(void);
void play_win_audio(void);
void play_fail_audio(void);
void completionCallback(SystemSoundID  soundID, void *clientData);
void current_output(int wordsize, char correct_input[]);
void basicman(void);
void printman(int counter);
int hangman(void)
{
    system("clear");
    // Asking the user for the number of letters the word must be
    int wordsize, runitback, winner;
    printYellow("Welcome to Hangman!\n");
    printYellow("You are allowed 7 mistakes or the man shall hang!\n");
    do
    {
        if (runitback == 1)
        {
            system("clear");
        }
        wordsize = 0, runitback = 0, winner = 0;
        printYellow("How many letters should the word contain (5-8): ");
        while (1)
        { 
            if (scanf("%i", &wordsize) == 1 && wordsize >= 5 && wordsize <= 8)
            {
                break; // Valid input, break out of the loop
            }
            else
            {
                // Clear the input buffer
                while (getchar() != '\n');
                printYellow("Error: wordsize must be either 5, 6, 7, or 8\n");
                printYellow("How many letters should the word contain: ");
            }
        }

        while (wordsize < 5 || wordsize > 8)
        {
            printYellow("Error: wordsize must be either 5, 6, 7, or 8\n");
            printYellow("How many letters should the word contain: ");
            scanf("%i", &wordsize);
            printYellow("Understood\n");
        }

        // Printing the basic hangman
        printYellow("\n");
        basicman();
        
        // Making an array for correct user input and making it empty
        char correct_input[wordsize + 1];   // 6    0 1 2 3 4 5
        for (int i = 0; i < wordsize; i++)
        {
            correct_input[i] = ' ';
        }
        correct_input[wordsize] = '\0';
        current_output(wordsize, correct_input);

        // open correct file, each file has exactly LISTSIZE words
        char wl_filename[10];
        sprintf(wl_filename, "%i.txt", wordsize);   // stores 5.txt or 6.txt whatever in wl_filename
        FILE *wordlist = fopen(wl_filename, "r");   // uses the name from wl_filename to write from that file into a file called wordlist
        if (wordlist == NULL)
        {
            printYellow("Error opening file %s.\n", wl_filename);
            return 1;
        }

        // load word file into an array options of size LISTSIZE
        char options[LISTSIZE][wordsize + 1];

        for (int i = 0; i < LISTSIZE; i++)
        {
            fscanf(wordlist, "%s", options[i]);
        }
        fclose(wordlist);

        // pseudorandomly select a word for this game
        srand(time(NULL));
        char choice[wordsize + 1];
        int random = rand() % LISTSIZE; // generates a random number which I then use as index for options
        for (int i = 0; i < wordsize; i++)
        {
            choice[i] = options[random][i];
        }
        choice[wordsize] = '\0';

        // Declaring variables and asking user for guess
        char guess;
        const int mistakes = 7;
        int excluded[26], included[26];
        int included_counter = 0, excluded_counter = 0;
        int present = 0, counter = 0;
        printYellow("%s\n", choice);
        printYellow("Enter a letter: ");
        scanf(" %c", &guess);

        // checking if it is alphabet or not
        if (isalpha(guess) == 0)
        {
            printYellow("The character you entered is not a letter\n");
        }

        while(isalpha(guess) == 0)
        {
            printYellow("Enter a letter: ");
            scanf(" %c", &guess);
        }
        printYellow("\n");
        guess = tolower(guess);

        while (counter < 7)
        {
            present = 0; // Reset present for each new guess

            for (int i = 0; i < wordsize; i++)  
            {
                if (guess == choice[i])
                {
                    present = 1;    // Indicates that the guessed char is in the choice
                    correct_input[i] = guess; // Store it in the correct position
                }
            }

            int compare = strcmp(choice, correct_input);
            if (compare == 0)
            {
                current_output(wordsize, correct_input);
                printGreen("CONGRATULATIONS!!!\n");
                printGreen("YOU HAVE WON YOU CHEEKY MAN!!!\n");
                winner = 1;
                play_win_audio();
                printf("\n");
                printYellow("Enter 1 to play again, else enter 0: ");
                scanf("%i", &runitback);
                if (runitback == 1)
                {
                    break;
                }
                else
                {
                    printf("\n\n\n");
                    printYellow("Thanks for playing\n");
                    sleep(3);
                    system("clear");
                    return 0;
                }
            }

            if (present == 1)
            {
                included[included_counter++] = guess;
                play_correct_audio();
                current_output(wordsize, correct_input);
            }
            else
            {
                excluded[excluded_counter++] = guess;
                counter++;

                // Print excluded letters
                for (int j = 0; j < excluded_counter; j++)
                {
                    if (j == 0)
                    {
                        printYellowWoSpaces("%c", excluded[j]);
                    } else
                    {
                        printYellowWoSpaces(", %c", excluded[j]);
                    }
                }
                printYellowWoSpaces(" is excluded\n\n");
            }
    
            // output
            if (present == 1)
            {
                included_counter++;
            }

            if (present == 0)
            {
                play_ghalat_hai_boss_audio();
                printman(counter);
            }

            // Taking user input every time after the first time
            int previous = 0;
            while (counter < 7 || previous == 1)
            {
                previous = 0;
                printYellow("\n");
                printYellow("Enter another letter: ");
                scanf(" %c", &guess);
                guess = tolower(guess);
                printYellow("\n");
                if (isalpha(guess) == 0)
                {
                    printYellow("The character you entered is not a letter\n");
                    while(isalpha(guess) == 0)
                    {
                        printYellow("Enter a letter: ");
                        scanf(" %c", &guess);
                    }
                }

                for (int c = 0; c < excluded_counter; c++)
                {
                    if (guess == excluded[c])
                    {
                        previous = 1;
                        printYellow("Please enter a new letter\n");
                        break;
                    }
                }
                if (previous == 1)
                {
                    continue;
                }
                for (int c = 0; c < included_counter; c++)
                {
                    if (guess == included[c])
                    {
                        previous = 1;
                        printYellow("Please enter a new letter\n");
                        break;
                    }
                }
                if (previous == 0)
                {
                    break;
                }
            }
        }
        if (winner == 0)
        {
            printRed("You have lost\n");
            printRed("You are indeed a loser\n\n");
            printYellow("The word was \"%s\"\n", choice);
            play_fail_audio();
            printf("\n\n");
            printYellow("Enter 1 to play again, else enter 0: ");
            scanf("%i", &runitback);
        }
    }
    while (runitback == 1);
    printf("\n\n\n");
    printYellow("Thanks for playing\n");
    sleep(3);
    system("clear");    
    return 0;
}

void basicman(void)
{
    printYellow("  +---+\n");
    printYellow("  |   |\n");
    printYellow("      |\n");
    printYellow("      |\n");
    printYellow("      |\n");
    printYellow("      |\n");
    printYellow("=========\n\n");
}
void current_output(int wordsize, char correct_input[])
{
    char line1[1024] = {0}; // Buffer for the first line
    char line2[1024] = {0}; // Buffer for the second line

    // Building the first line
    for (int z = 0; z < wordsize; z++) {
        char temp[4];
        snprintf(temp, sizeof(temp), " %c ", correct_input[z]);
        strcat(line1, temp);
    }

    // Building the second line
    for (int z = 0; z < wordsize; z++) {
        strcat(line2, "__ ");
    }

    // Printing the lines
    printYellow(line1);
    printYellow("\n");
    printYellow(line2);
    printYellow("\n\n");
}
void printman(int counter)
{
    if (counter == 1)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed("      |\n");
        printRed("      |\n");
        printRed("      |\n");
        printRed("=========\n");
    }
    else if (counter == 2)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed("  |   |\n");
        printRed("      |\n");
        printRed("      |\n");
        printRed("=========\n");
    }
    else if (counter == 3)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed(" \\|   |\n");
        printRed("      |\n");
        printRed("      |\n");
        printRed("=========\n");
    }
    else if (counter == 4)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed(" \\|/  |\n");
        printRed("      |\n");
        printRed("      |\n");
        printRed("=========\n");
    }
    else if (counter == 5)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed(" \\|/  |\n");
        printRed("  |   |\n");
        printRed("      |\n");
        printRed("=========\n");
    }
    else if (counter == 6)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed(" \\|/  |\n");
        printRed("  |   |\n");
        printRed("   \\  |\n");
            printRed("=========\n");
    }
    else if (counter == 7)
    {
        printRed("  +---+\n");
        printRed("  |   |\n");
        printRed("  0   |\n");
        printRed(" \\|/  |\n");
        printRed("  |   |\n");
        printRed(" / \\  |\n");
        printRed("=========\n");
    }

}
void completionCallback(SystemSoundID  soundID, void *clientData)
{
    CFRunLoopStop(CFRunLoopGetCurrent());
}

void play_fail_audio(void)
{
    const char *file_path = "fail.wav"; // Replace this with the path to your WAV file
    audio_shorten(file_path);
}
void play_correct_audio(void)
{
    const char *file_path = "correct.wav"; // Replace this with the path to your WAV file
    audio_shorten(file_path);
}
void play_ghalat_hai_boss_audio(void)
{
    const char *file_path = "ghalat.wav"; // Replace this with the path to your WAV file
    audio_shorten(file_path);
}
void *playAudio(void *arg) {
    SystemSoundID soundID = *((SystemSoundID*)arg);

    AudioServicesPlaySystemSound(soundID);
    return NULL;
}
void play_win_audio(void) {
    const char *file_path = "win.wav"; // Replace with your WAV file path

    CFURLRef fileURL = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)file_path, strlen(file_path), false);
    SystemSoundID soundID;
    OSStatus status = AudioServicesCreateSystemSoundID(fileURL, &soundID);
    
    if (status != kAudioServicesNoError) {
        printYellow("Error occurred while loading sound file\n");
        return;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, playAudio, &soundID);

    usleep(9000000); // 7 seconds in microseconds

    AudioServicesDisposeSystemSoundID(soundID); // Stop the sound
    CFRelease(fileURL);

    pthread_cancel(thread_id); // Cancel the thread if it's still running
}
void audio_shorten(const char *file_path)
{
    CFURLRef fileURL = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)file_path, strlen(file_path), false);
    SystemSoundID soundID;
    OSStatus status = AudioServicesCreateSystemSoundID(fileURL, &soundID);
    
    if (status != kAudioServicesNoError) {
        printYellow("Error occurred while loading sound file\n");
        //return 1;
    }

    AudioServicesAddSystemSoundCompletion(soundID, NULL, NULL, completionCallback, NULL);
    
    AudioServicesPlaySystemSound(soundID);
    
    // Run the loop to wait for the sound to finish playing
    CFRunLoopRun();

    // Clean up
    AudioServicesDisposeSystemSoundID(soundID);
    CFRelease(fileURL);
}
void printYellow(const char *format, ...)
{
    char buffer[1024]; // Large buffer to hold the formatted string
    va_list args;
    
    // Using vsnprintf to format the string
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);
    int spaces = (TERMINAL_WIDTH - len) / 2;

    // Print leading spaces
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }

    // Print text in yellow
    printf("%s%s%s", YELLOW, buffer, RESET_COLOR); // Removed '\n' from here
}
void printBlue(const char *format, ...)
{
    char buffer[1024]; // Large buffer to hold the formatted string
    va_list args;
    
    // Using vsnprintf to format the string
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);
    int spaces = (TERMINAL_WIDTH - len) / 2;

    // Print leading spaces
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }

    // Print text in blue
    printf("%s%s%s", BLUE, buffer, RESET_COLOR); // Removed '\n' from here
}
void printGreen(const char *format, ...)
{
    char buffer[1024]; // Large buffer to hold the formatted string
    va_list args;
    
    // Using vsnprintf to format the string
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);
    int spaces = (TERMINAL_WIDTH - len) / 2;

    // Print leading spaces
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }

    // Print text in green
    printf("%s%s%s", BRIGHT_GREEN, buffer, RESET_COLOR); // Removed '\n' from here
}
void printRed(const char *format, ...)
{
    char buffer[1024]; // Large buffer to hold the formatted string
    va_list args;
    
    // Using vsnprintf to format the string
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);
    int spaces = (TERMINAL_WIDTH - len) / 2;

    // Print leading spaces
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }

    // Print text in green
    printf("%s%s%s", RED, buffer, RESET_COLOR); // Removed '\n' from here
}
void printYellowWoSpaces(const char *format, ...)
{
    char buffer[1024]; // Large buffer to hold the formatted string
    va_list args;
    
    // Using vsnprintf to format the string
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);

    // Print text in yellow
    printf("%s%s%s", YELLOW, buffer, RESET_COLOR); // Removed '\n' from here
}