#include <stdio.h>
#include "hangman.c"
#include "tictactoe.c"
#include "scrabble.c"

Mix_Music *music;
// gcc -o main main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer -framework AudioToolbox -framework CoreFoundation

void setMusicVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

void playBackgroundMusic(int volume)
{
    music = Mix_LoadMUS("ishq.mp3");  // Load the music file

    if (music == NULL)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    else
    {
        Mix_VolumeMusic(volume);  // Set the volume
        Mix_PlayMusic(music, -1);  // Play the music
    }
}

volatile sig_atomic_t quit = 0;

// Signal handler
void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        // Cleanup and close SDL_mixer
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        Mix_Quit();

        // Close SDL
        SDL_Quit();

        // Exit the program
        exit(0);
    }
}

int main(void)
{
    int volume = 2;
    // Register signal handler
    signal(SIGINT, handle_signal);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    // Play background music
    playBackgroundMusic(volume);
    system("clear");
    int selection ;
    printYellow("Welcome to Miniclip cheap version!\n\n\n");
    printYellow("We have multiple games in store for you!\n\n\n");
    printYellow("1. Tictactoe\n");
    printYellow("2. Score More\n");
    printYellow("3. Hangman\n");
    printYellow("4. Quit\n");
    printf("\n\n");
    printYellow("Enter the number for whichever game you'd like to play: ");
    scanf(" %i", &selection);
    while (selection != 1 && selection != 2 && selection != 3 && selection != 4)
    {
        printYellow("Wrong number entered. Please enter from the given choice: ");
        scanf(" %i", &selection);
    }

    if (selection == 1)
    {
        return tictactoe();
    }
    else if(selection == 2)
    {
        return scrabble(POINTS);
    }
    else if(selection == 3)
    {
        return hangman();
    }
    else if(selection == 4)
    {
        printf("\n\n");
        return 0;
    }
        // Cleanup
    Mix_FreeMusic(music); // Make sure you define 'music' as a global or pass it properly
    Mix_Quit();
    SDL_Quit();
    return 0;
}

// Mix_OpenAudio, Mix_LoadMUS, Mix_PlayMusic, Mix_FreeMusic, Mix_Quit:
// These functions are part of the SDL_mixer library, which is included via <SDL2/SDL_mixer.h>.

// SDL_Init, AudioServicesPlaySystemSound, AudioServicesCreateSystemSoundID, AudioServicesAddSystemSoundCompletion,
// AudioServicesDisposeSystemSoundID, CFURLCreateFromFileSystemRepresentation, CFRunLoopRun, CFRunLoopStop, CFRelease:
// These functions are part of the SDL and CoreFoundation libraries, which are included via <SDL2.h> and <AudioToolbox/AudioToolbox.h> headers

// printf scanf sprintf fopen fscanf fclose: <stdio.h> header.
// sleep:  <unistd.h> header.
// system srand: <stdlib.h> header.
// time: <time.h> header.
// strcpy strcmp strcat strlen:  <string.h> header.
// isalpha tolower: <ctype.h> header.
