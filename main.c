#include<stdio.h>          // Header file for input and output functions
#include "wynk.c"          // Includes the functions and data structures implemented in wynk.c
#include "pthread_unistd.h"
#include <windows.h>       // Provides Sleep() and Windows console functions

// ---------------- WELCOME SCREEN ----------------

// Displays the Wynk Music welcome logo with colors and animation
void welcome()
{
    unsigned int mSeconds = 100000;  // Delay value used for the welcome animation

    printf("________________________________________________________________________________\n");

    printf("\033[0;31m");  // Changes the text color to red

    // Prints the Wynk Music ASCII logo
    printf("__        __           _         __  __           _              _                 \n");
    Sleep(mSeconds / 1000);  // Pauses execution for a short time

    printf("\\ \\      / /   _ _ __ | | __    |  \\/  |_   _ ___(_) ___       / \\   _ __  _ __   \n");
    Sleep(mSeconds / 1000);

    printf(" \\ \\ /\\ / / | | | '_ \\| |/ /    | |\\/| | | | / __| |/ __|      / _ \\ | '_ \\| '_ \\  \n");
    Sleep(mSeconds / 1000);

    printf("  \\ V  V /| |_| | | | |   <     | |  | | |_| \\__ \\ | (__      / ___ \\| |_) | |_) | \n");
    Sleep(mSeconds / 1000);

    printf("   \\_/\\_/  \\__, |_| |_|\\_\\    |_|  |_|\\__,_|___/_|\\___|    /_/   \\_\\ .__/| .__/  \n");
    Sleep(mSeconds / 1000);

    printf("           |___/                                                     |_|   |_|     \n");

    printf("\033[0m");  // Resets the text color to default

    printf("________________________________________________________________________________\n\n");

    Sleep(1000);  // Waits for one second
}


// ---------------- LOGIN / REGISTRATION MENU ----------------

// Displays the menu for existing and new users
void menuSignin()
{
    printf("\033[0;32m");  // Changes text color to green

    printf("-----------------------");
    printf("\n         MENU\n");
    printf("-----------------------\n");

    printf("1: Log in\n");     // Option to log in
    printf("2: Sign In\n");   // Option to register

    printf("-----------------------\n");

    printf("\033[0m");  // Resets text color
}


// ---------------- PLAYLIST MENU ----------------

// Displays all available playlist operations
void menu()
{
    printf("\033[0;93m");  // Changes text color to yellow

    printf("\n-----------------------");
    printf("\n         MENU\n");
    printf("-----------------------\n");

    printf("\033[0m");  // Resets text color

    printf("\n1.Add song to the playlist\n\n");
    printf("2.Display all songs of the playlist\n\n");
    printf("3.Play first song of playlist\n\n");
    printf("4.Play last song of playlist\n\n");
    printf("5.Display songs by language\n\n");
    printf("6.Play previous song\n\n");
    printf("7.Play next song\n\n");
    printf("8.Count the number of songs in the playlist\n\n");
    printf("9.Search a song and play\n\n");
    printf("10.Delete a song from the playlist\n\n");
    printf("11.Display currently playing song\n\n");
    printf("12.Sort by year\n\n");
    printf("13.Display recently searched songs\n\n");
    printf("14.Exit\n");
}


// ---------------- HOME SCREEN MENU ----------------

// Displays the options available on the home screen
void menu2()
{
    printf("\033[0;35m");  // Changes text color to purple

    printf("\n1.Select a song to play\n");
    printf("\n2.Create a playlist\n\n");
    printf("3.Exit\n");

    printf("\033[0m");  // Resets text color
}


// ---------------- HOME SCREEN ----------------

// Displays the home screen, complete music library and home menu
void feed()
{
    printf("\n---------------------HOME---------------------\n");

    display();  // Calls display() to show all songs from the music library

    menu2();    // Displays the home screen options
}


// ---------------- MAIN FUNCTION ----------------

// Execution of the program starts from main()
int main()
{
    // Sets Windows console input and output encoding to UTF-8
    // This helps display special characters and symbols correctly
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // choice    = Stores the user's menu selection
    // logstatus = Stores login status (0 = not logged in, 1 = logged in)
    // x         = Stores login menu selection
    // flag      = Controls entry into the playlist menu
    int choice, logstatus=0, x, flag=0;

    welcome();  // Displays the welcome screen

    // Loads the user's previously saved playlist from playlist.txt
    // and creates its circular doubly linked list
    readData();

    // Loads all available songs from songs.txt
    // and creates the complete music library linked list
    readData2();


    // ---------------- USER AUTHENTICATION ----------------

    // Repeats the login/registration menu until login is successful
    while(logstatus!=1)
    {
        menuSignin();  // Displays login and registration options

        scanf("%d", &x);  // Takes the user's choice

        switch (x)
        {
        case 1:
            // Calls login() to verify the username and password
            // Returns 1 if login is successful, otherwise 0
            logstatus=login();
            break;

        case 2:
            // Calls registration() to register a new user
            registration();
            break;

        default:
            // Handles invalid menu choices
            printf("Please enter valid input..\n");
            break;
        }
    }


    // ---------------- HOME SCREEN LOOP ----------------

    // Continuously displays the home screen until the program exits
    while(1)
    {
        feed();  // Displays the music library and home menu

        printf("\nEnter choice: ");
        scanf("%d",&choice);  // Takes the user's home screen choice

        switch(choice)
        {
        case 1:
            // Searches for a song in the complete music library
            // and simulates playing the selected song
            search_and_play2();
            break;

        case 2:
            flag=1; /*head=0;*/ goto playlist;
            break;

        case 3:
            // Terminates the application
            exit(0);
            break;

        default:
            // Handles invalid input
            printf("Invalid input\n");
        }
    }


// ---------------- USER PLAYLIST OPERATIONS ----------------

// Label used to jump to the playlist section using goto
playlist:

    // Executes the playlist menu while flag is non-zero
    while(flag)
    {
        menu();  // Displays all playlist operations

        printf("\nEnter choice: ");
        scanf("%d",&choice);  // Takes the user's playlist menu choice

        switch(choice)
        {
        case 1:
            // Adds a selected song to the user's playlist
            insert_song();
            break;

        case 2:
            // Displays all songs currently present in the playlist
            display_playlist();
            break;

        case 3:
            // Selects the first song as the current song
            // Stores the returned node address in curr_song
            curr_song = play_first_song();
            break;

        case 4:
            // Selects the last song as the current song
            curr_song = play_last_song();
            break;

        case 5:
            // Displays songs according to the language entered by the user
            display_by_language();
            break;

        case 6:
            // Moves to the previous song in the playlist
            // Updates curr_song with the returned node
            curr_song = play_prev(curr_song);
            break;

        case 7:
            // Moves to the next song in the playlist
            // Updates curr_song with the returned node
            curr_song = play_next(curr_song);
            break;

        case 8:
            // Calls count() to calculate the total number of playlist songs
            printf("%d",count());
            break;

        case 9:
            // Searches for a song in the playlist and simulates playback
            // Stores the returned song node in curr_song
            curr_song = search_and_play();
            break;

        case 10:
            // Deletes a selected song from the playlist
            delete_song();
            break;

        case 11:
            // Displays the currently selected/playing song
            display_curr_song();
            break;

        case 12:
            // Sorts playlist songs according to their release year
            // Uses Bubble Sort internally
            sort();
            break;

        case 13:
            // Displays the recently searched song names
            display_recently_searched();
            break;

        case 14:
            // Exits the application
            exit(0);
        }
    }

    return 0;  // Indicates successful program termination
}