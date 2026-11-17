#include<stdio.h>      // For printf(), scanf(), file input/output functions
#include<stdlib.h>     // For malloc(), free(), exit() and remove()
#include<string.h>     // For strcpy(), strcmp() and strcasecmp()
#include "pthread_unistd.h"
#include <windows.h>   // For Sleep() function

// Structure representing one song node in a circular doubly linked list
struct node
{
    struct node *prev;     // Pointer to the previous song
    struct node *next;     // Pointer to the next song
    char song[30];         // Stores the song name
    char artist[30];       // Stores the artist name
    char language[20];     // Stores the song language
    int likes;             // Stores the number of likes
    int year;              // Stores the release year
};

// Structure used to store a recently searched song name
struct name
{
    char song[30];
};

// Structure containing an array of recently searched songs
struct search
{
    struct name array[100];    // Can store up to 100 song names
};

// Structure used for user registration and login
struct login
{
    char name[30];             // User's full name
    char username[30];         // User's username
    char password[30];         // User's password
};

// Global pointers used throughout the program
struct node *head=NULL, *temp=NULL, *curr_song=NULL,
            *temp1=NULL, *temp2=NULL, *tail=NULL, *head2=NULL;

// head  = Starting node of the user's playlist
// head2 = Starting node of the complete music library
// curr_song = Points to the currently selected song
// temp, temp1, temp2 = Temporary pointers used for traversal and operations

struct search x;      // Global structure for storing search history
int f=0;              // Tracks the number of recently searched songs


// ---------------- USER REGISTRATION ----------------

// Registers a new user and stores their details in users.txt
void registration()
{
    struct login l;    // Structure variable to store user details
    FILE *log;         // File pointer

    log = fopen("users.txt", "w");  // Opens file in write mode

    if(!log)
    {
        fputs("Error at opening File!", stderr);
        exit(1);       // Terminates program if file cannot be opened
    }

    printf("Enter your name: ");
    fflush(stdin);
    scanf("%[^\n]s",l.name);

    printf("Username: ");
    fflush(stdin);
    scanf("%[^\n]s",l.username);

    printf("Password: ");
    fflush(stdin);
    scanf("%[^\n]s",l.password);

    // Writes the user's name, username and password into the file
    fprintf(log,"\n%s %s %s",l.name, l.username, l.password);

    fclose(log);       // Closes the file after writing

    printf("\nConfirming details...\n...\nWelcome, %s!\n\n",l.name);
    printf("\nRegistration Successful!\n");
}


// ---------------- USER LOGIN ----------------

// Checks whether the entered username and password are correct
int login()
{
    char username[30], password[20], u[30], p[30], x[30];
    FILE *log;

    log = fopen("users.txt","r");   // Opens user details file in read mode

    if (!log)
    {
        fputs("Please sign up before logging in", stderr);
        exit(1);
    }

    printf("\nPlease Enter your login credentials below\n\n");

    printf("Username:  ");
    fflush(stdin);
    scanf("%[^\n]s",username);

    printf("\nPassword: ");
    fflush(stdin);
    scanf("%[^\n]s",password);

    // Reads the saved user details and compares credentials
    while(!feof(log))
    {
        fscanf(log,"%s%s%s",x,u,p);

        // strcmp() returns 0 when both strings are equal
        if(strcmp(username,u)==0 && strcmp(password,p)==0)
        {
            printf("\nSuccessful Login\n");
            return 1;     // Returns 1 when login is successful
        }
        else
        {
            printf("\nIncorrect Login Details\nPlease enter the correct credentials\n");
        }
    }

    fclose(log);

    return 0;             // Returns 0 when login fails
}


// ---------------- SAVE PLAYLIST DATA ----------------

// Saves the current playlist into playlist.txt
void writeData()
{
    if(head)
    {
        FILE *fp = fopen("playlist.txt", "w"); // Opens playlist file in write mode

        temp = head;

        // Traverses the circular linked list and writes every song
        do
        {
            fprintf(fp,"\n%s %s %s %d %d",
                    temp->song, temp->artist, temp->language,
                    temp->likes, temp->year);

            temp = temp->next;   // Moves to the next node
        }
        while (temp!=head);      // Stops after reaching the first node again

        fclose(fp);
    }
    else
    {
        // Removes the playlist file if the playlist is empty
        remove("playlist.txt");
    }
}


// ---------------- LOAD COMPLETE MUSIC LIBRARY ----------------

// Reads songs.txt and creates the complete music library linked list
void readData2()
{
    char s[30], a[30], l[20];
    int x, y;
    FILE *fp=NULL;

    fp=fopen("songs.txt","r");    // Opens the song database

    if(fp==NULL)
    {
        return;                   // Returns if file does not exist
    }
    else
    {
        do
        {
            // Reads song name, artist, language, likes and year
            fscanf(fp,"%s%s%s%d%d",s,a,l,&x,&y);

            // If the list is empty, create the first node
            if(head2==NULL)
            {
                head2 = (struct node *)malloc(sizeof(struct node));

                strcpy(head2->song,s);
                strcpy(head2->artist,a);
                strcpy(head2->language,l);

                head2->likes = x;
                head2->year = y;

                // First node points to itself in both directions
                // This makes the list circular
                head2->next = head2;
                head2->prev = head2;
            }
            else
            {
                struct node *newnode;

                // Dynamically allocates memory for a new song node
                newnode = (struct node *)malloc(sizeof(struct node));

                temp=head2;

                // Finds the last node of the circular linked list
                while(temp->next!=head2)
                {
                    temp=temp->next;
                }

                // Copies song details into the new node
                strcpy(newnode->song,s);
                strcpy(newnode->artist,a);
                strcpy(newnode->language,l);

                newnode->likes = x;
                newnode->year = y;

                // Inserts the new node at the end of the list
                newnode->next=head2;
                head2->prev=newnode;

                temp->next=newnode;
                newnode->prev=temp;
            }
        }
        while (!feof(fp));    // Continues until the end of the file

        fclose(fp);
    }
}


// ---------------- LOAD USER PLAYLIST ----------------

// Reads playlist.txt and creates the user's playlist linked list
void readData()
{
    char s[30], a[30], l[20];
    int x, y;
    FILE *fp=NULL;

    fp=fopen("playlist.txt","r");  // Opens the saved playlist

    if(fp==NULL)
    {
        return;                    // No saved playlist exists
    }
    else
    {
        do
        {
            // Reads details of one song from the file
            fscanf(fp,"%s%s%s%d%d",s,a,l,&x,&y);

            // Creates the first node if the playlist is empty
            if(head==NULL)
            {
                head = (struct node *)malloc(sizeof(struct node));

                strcpy(head->song,s);
                strcpy(head->artist,a);
                strcpy(head->language,l);

                head->likes = x;
                head->year = y;

                // Makes the first node circular
                head->next = head;
                head->prev = head;
            }
            else
            {
                struct node *newnode;

                // Allocates memory for another song
                newnode = (struct node *)malloc(sizeof(struct node));

                temp=head;

                // Traverses to the last node
                while(temp->next!=head)
                {
                    temp=temp->next;
                }

                // Copies song details into the new node
                strcpy(newnode->song,s);
                strcpy(newnode->artist,a);
                strcpy(newnode->language,l);

                newnode->likes = x;
                newnode->year = y;

                // Connects the new node at the end of the list
                newnode->next=head;
                head->prev=newnode;

                temp->next=newnode;
                newnode->prev=temp;
            }
        }
        while (!feof(fp));

        fclose(fp);
    }
}


// ---------------- CREATE A SONG NODE ----------------

// Searches for a song in songs.txt and returns a newly created node
struct node *getnode()
{
    char s[30], a[30], l[20], SONG[30];
    int x, y, flag=0;

    struct node *newnode;

    // Allocates memory for the new node
    newnode = (struct node *)malloc(sizeof(struct node));

    printf("\nEnter the name of the song: ");
    fflush(stdin);
    scanf("%[^\n]s",SONG);

    FILE *fp = fopen("songs.txt", "r");

    // Searches through the song database
    do
    {
        fscanf(fp,"%s%s%s%d%d",s,a,l,&x,&y);

        // Checks whether the entered song name matches
        if(strcasecmp(SONG,s)==0)
        {
            flag=1;

            // Copies the song details into the new node
            strcpy(newnode->song,s);
            strcpy(newnode->artist,a);
            strcpy(newnode->language,l);

            newnode->likes = x;
            newnode->year = y;

            // Initially makes the node point to itself
            newnode->prev = newnode->next = newnode;
        }
    }
    while (!feof(fp));

    if(!flag)
    {
        printf("Song not found :(\n");
        return NULL;
    }

    fclose(fp);

    return newnode;       // Returns the newly created node
}


// ---------------- INSERT SONG INTO PLAYLIST ----------------

// Adds a song to the user's circular doubly linked playlist
void insert_song()
{
    int flag=0;

    // Calls getnode() to find and create the selected song node
    struct node *newnode=getnode();

    if(!newnode)
    {
        printf("Cannot insert\n");
        return;
    }

    // Case 1: Playlist is empty
    if(head==NULL)
    {
        head=newnode;
    }

    // Case 2: Playlist contains only one node
    else if(head->next==head)
    {
        // Checks whether the song is already present
        if(!strcasecmp(head->song,newnode->song))
        {
            printf("\nSong already in the playlist");

            free(newnode);   // Frees memory because duplicate is not inserted
            flag=1;
        }

        if(flag==0)
        {
            // Connects the new node with the existing node
            newnode->next=head;
            head->prev=newnode;

            head->next=newnode;
            newnode->prev=head;
        }
    }

    // Case 3: Playlist contains multiple nodes
    else
    {
        temp=head;

        // Traverses the list to check for duplicate songs
        do
        {
            if(!strcasecmp(temp->song,newnode->song))
            {
                printf("\nSong already in the playlist");

                free(newnode);
                flag=1;
                break;
            }

            temp=temp->next;
        }
        while(temp!=head);

        // Moves temp to the last node
        temp=temp->prev;

        if(flag==0)
        {
            // Inserts the new node at the end of the playlist
            newnode->next=head;
            head->prev=newnode;

            temp->next=newnode;
            newnode->prev=temp;
        }
    }

    writeData();       // Saves the updated playlist
}


// ---------------- DELETE SONG FROM PLAYLIST ----------------

// Deletes a selected song from the user's playlist
void delete_song()
{
    int d=0;
    char song[30];

    printf("\nEnter the name of the song you want to delete : ");
    fflush(stdin);
    scanf("%[^\n]s",song);

    // Checks whether the playlist is empty
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
    }

    // Case 1: Playlist contains only one node
    else if(head->next == head)
    {
        temp=head;

        if(!strcasecmp(song,temp->song))
        {
            printf("\n%s deleted",song);
            d=1;

            // If the deleted song is currently selected, reset curr_song
            if(curr_song==temp)
            {
                curr_song = NULL;
            }

            head=NULL;       // Playlist becomes empty
            free(temp);      // Releases allocated memory
        }
    }

    // Case 2: Playlist contains multiple nodes
    else
    {
        // Checks whether the first node is the song to delete
        if(!strcasecmp(head->song,song))
        {
            temp=head;

            if(curr_song==temp)
            {
                curr_song=NULL;
            }

            // Stores the previous and next nodes
            temp1=temp->prev;
            temp2=temp->next;

            // Connects the previous node directly to the next node
            temp1->next=temp2;
            temp2->prev=temp1;

            temp->next=NULL;
            temp->prev=NULL;

            free(temp);      // Deletes the selected node

            head=temp2;      // Updates the head pointer

            printf("\n%s deleted",song);
            d=1;
        }

        // Searches for the song in the remaining nodes
        else
        {
            temp=head->next;

            do
            {
                if(!strcasecmp(temp->song,song))
                {
                    if(curr_song==temp)
                    {
                        curr_song=NULL;
                    }

                    temp1=temp->prev;
                    temp2=temp->next;

                    // Bypasses the node being deleted
                    temp1->next=temp2;
                    temp2->prev=temp1;

                    temp->next=NULL;
                    temp->prev=NULL;

                    free(temp);

                    printf("\n%s deleted",song);
                    d=1;
                    break;
                }

                temp=temp->next;
            }
            while(temp!=head);
        }
    }

    writeData();       // Saves the playlist after deletion
}


// ---------------- DISPLAY USER PLAYLIST ----------------

// Displays all songs present in the user's playlist
void display_playlist()
{
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
    }
    else
    {
        temp=head;

        // Traverses the list until the last node
        while(temp->next!=head)
        {
            printf("-----------------------\n");
            printf("Song : %s\n",temp->song);
            printf("Artist : %s\n",temp->artist);
            printf("Language : %s\n",temp->language);
            printf("Likes : %d\n",temp->likes);
            printf("Year : %d\n\n",temp->year);
            printf("-----------------------\n");

            temp=temp->next;
        }

        // Displays the last node separately
        printf("-----------------------\n");
        printf("Song : %s\n",temp->song);
        printf("Artist : %s\n",temp->artist);
        printf("Language : %s\n",temp->language);
        printf("Likes : %d\n",temp->likes);
        printf("Year : %d\n\n",temp->year);
        printf("-----------------------\n");
    }
}


// ---------------- DISPLAY COMPLETE MUSIC LIBRARY ----------------

// Displays all songs stored in the music library (head2)
void display()
{
    if(head2==NULL)
    {
        printf("\nThe playlist is empty");
    }
    else
    {
        temp=head2;

        // Traverses the complete music library
        while(temp->next!=head2)
        {
            printf("-----------------------\n");
            printf("Song : %s\n",temp->song);
            printf("Artist : %s\n",temp->artist);
            printf("Language : %s\n",temp->language);
            printf("Likes : %d\n",temp->likes);
            printf("Year : %d\n\n",temp->year);
            printf("-----------------------\n");

            temp=temp->next;
        }

        // Displays the last song
        printf("-----------------------\n");
        printf("Song : %s\n",temp->song);
        printf("Artist : %s\n",temp->artist);
        printf("Language : %s\n",temp->language);
        printf("Likes : %d\n",temp->likes);
        printf("Year : %d\n\n",temp->year);
        printf("-----------------------\n");
    }
}


// ---------------- PLAY FIRST SONG ----------------

// Selects and displays the first song of the playlist
struct node *play_first_song()
{
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
        return NULL;
    }
    else
    {
        temp = head;

        curr_song = temp;     // Updates the current song pointer

        printf("\nNow Playing : %s",temp->song);

        return temp;          // Returns the selected song node
    }
}


// ---------------- PLAY LAST SONG ----------------

// Selects and displays the last song of the playlist
struct node *play_last_song()
{
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
        return NULL;
    }
    else
    {
        temp = head;

        // In a circular doubly linked list, head->prev is the last node
        curr_song = temp->prev;

        printf("\nNow Playing : %s",(temp->prev)->song);

        return (temp->prev);
    }
}


// ---------------- FILTER SONGS BY LANGUAGE ----------------

// Displays songs that match the language entered by the user
void display_by_language()
{
    char language[20];

    printf("\nEnter the language : ");
    fflush(stdin);
    scanf("%[^\n]s",language);

    if(head==NULL)
    {
        printf("\nThe playlist is empty");
    }
    else
    {
        temp=head;

        // Checks the language of every node except the last one
        while(temp->next!=head)
        {
            if(!strcasecmp(language,temp->language))
            {
                printf("\n%s",temp->song);
            }

            temp=temp->next;
        }

        // Checks the last node separately
        if(!strcasecmp(language,temp->language))
        {
            printf("\n%s",temp->song);
        }
    }
}


// ---------------- PLAY PREVIOUS SONG ----------------

// Finds the current song and moves to its previous song
struct node *play_prev(struct node *curr_song)
{
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
        return NULL;
    }

    if(curr_song==NULL)
    {
        printf("\nPlay a song first");
        return NULL;
    }
    else
    {
        temp=head;

        // Searches for the currently selected song
        while(1)
        {
            if(!strcasecmp(temp->song,curr_song->song))
            {
                // Displays the previous song using the prev pointer
                printf("\nNow Playing : %s",(temp->prev)->song);
                break;
            }

            temp=temp->next;
        }

        // Returns the previous song node
        return (temp->prev);
    }
}


// ---------------- PLAY NEXT SONG ----------------

// Finds the current song and moves to its next song
struct node *play_next(struct node *curr_song)
{
    if(head==NULL)
    {
        printf("\nThe playlist is empty");
        return NULL;
    }

    if(curr_song==NULL)
    {
        printf("\nPlay a song first");
        return NULL;
    }
    else
    {
        temp=head;

        // Searches for the currently selected song
        while(1)
        {
            if(!strcasecmp(temp->song,curr_song->song))
            {
                // Displays the next song using the next pointer
                printf("\nNow Playing : %s",(temp->next)->song);
                break;
            }

            temp=temp->next;
        }

        // Returns the next song node
        return (temp->next);
    }
}


// ---------------- COUNT PLAYLIST SONGS ----------------

// Counts the total number of nodes in the playlist
int count()
{
    int c=0;

    if(head==NULL)
    {
        return 0;          // Returns zero if playlist is empty
    }
    else
    {
        temp=head;

        // Traverses the list and increments the counter
        while(temp->next!=head)
        {
            c++;
            temp=temp->next;
        }

        c++;               // Counts the last node

        return c;
    }
}


// ---------------- SEARCH AND PLAY FROM PLAYLIST ----------------

// Searches for a song in the user's playlist and simulates playback
struct node *search_and_play()
{
    unsigned int mSeconds = 100000;
    int found=0;
    char song[30];

    printf("\nEnter the name of the song you want to play : ");
    fflush(stdin);
    scanf("%[^\n]s",song);

    // Stores the searched song name in the recent-search array
    strcpy(x.array[f].song,song);
    f++;

    if(head==NULL)
    {
        printf("\nThe playlist is empty");
        return NULL;
    }
    else
    {
        temp=head;

        // Performs linear search through the playlist
        while(temp->next!=head)
        {
            if(!strcasecmp(song,temp->song))
            {
                printf("\033[0;34m");

                // Displays the selected song
                printf("\n▁ ▂ ▄ ▅ ▆ ▇ █ Now Playing █ ▇ ▆ ▅ ▄ ▂ ▁\n %s\n",temp->song);

                printf("\033[0m");

                // Pauses the program to simulate song playback
                Sleep(mSeconds / 1000);
                Sleep(1000);

                found = 1;
                break;
            }

            temp=temp->next;
        }

        // Checks the last node separately
        if(!found)
        {
            if(!strcasecmp(song,temp->song))
            {
                printf("\033[0;34m");
                printf("\n▁ ▂ ▄ ▅ ▆ ▇ █ Now Playing █ ▇ ▆ ▅ ▄ ▂ ▁\n %s\n",temp->song);
                printf("\033[0m");

                Sleep(mSeconds / 1000);
                Sleep(1000);

                found = 1;
            }
        }

        if(!found)
        {
            printf("\nSong not found in the playlist");

            return curr_song;    // Keeps the previous current-song pointer
        }
        else
        {
            return temp;          // Returns the found song node
        }
    }
}


// ---------------- SEARCH AND PLAY FROM COMPLETE LIBRARY ----------------

// Searches for a song in the complete music library
void search_and_play2()
{
    unsigned int mSeconds = 100000;
    int found=0;
    char song[30];

    printf("\nEnter the name of the song you want to play : ");
    fflush(stdin);
    scanf("%[^\n]s",song);

    // Adds the searched song to the recent-search array
    strcpy(x.array[f].song,song);
    f++;

    if(head2==NULL)
    {
        printf("\nThe playlist is empty");
    }
    else
    {
        temp=head2;

        // Performs linear search in the complete music library
        while(temp->next!=head2)
        {
            if(!strcasecmp(song,temp->song))
            {
                printf("\033[0;34m");
                printf("\n▁ ▂ ▄ ▅ ▆ ▇ █ Now Playing █ ▇ ▆ ▅ ▄ ▂ ▁\n %s\n",temp->song);
                printf("\033[0m");

                // Simulates playback using a delay
                Sleep(mSeconds / 1000);
                Sleep(1000);

                found = 1;
                break;
            }

            temp=temp->next;
        }

        // Checks the last node separately
        if(!found)
        {
            if(!strcasecmp(song,temp->song))
            {
                printf("\033[0;34m");
                printf("\n▁ ▂ ▄ ▅ ▆ ▇ █ Now Playing █ ▇ ▆ ▅ ▄ ▂ ▁\n %s\n",temp->song);
                printf("\033[0m");

                Sleep(mSeconds / 1000);
                Sleep(1000);

                found = 1;
            }
        }

        if(!found)
        {
            printf("\nSong not found in the playlist");
        }
    }
}


// ---------------- DISPLAY CURRENT SONG ----------------

// Displays the song currently pointed to by curr_song
void display_curr_song()
{
    if(curr_song==NULL)
    {
        printf("\nNo song is being played");
    }
    else
    {
        printf("\nNow Playing : %s",curr_song->song);
    }
}


// ---------------- SWAP SONG DETAILS ----------------

// Swaps the data of two nodes without changing their links
void swap(struct node *a, struct node *b)
{
    int x;
    char z[50];

    // Swaps release years
    x = a->year;
    a->year = b->year;
    b->year = x;

    // Swaps likes
    x = a->likes;
    a->likes = b->likes;
    b->likes = x;

    // Swaps song names using a temporary character array
    strcpy(z,a->song);
    strcpy(a->song,b->song);
    strcpy(b->song,z);

    // Swaps artist names
    strcpy(z,a->artist);
    strcpy(a->artist,b->artist);
    strcpy(b->artist,z);

    // Swaps languages
    strcpy(z,a->language);
    strcpy(a->language,b->language);
    strcpy(b->language,z);
}


// ---------------- SORT PLAYLIST ----------------

// Sorts the playlist by release year using Bubble Sort
void sort()
{
    if(head==NULL)
    {
        printf("\nNo songs in the playlist");
    }
    else if(head->next==head)
    {
        // A single-node playlist is already sorted
        printf("\nSongs have been sorted");
    }
    else
    {
        int i,j;

        // Outer loop controls the number of Bubble Sort passes
        for(i=0; i<count()-1; i++)
        {
            temp1=head;
            temp2=head->next;

            // Inner loop compares adjacent nodes
            for(j=0; j<count()-1-i; j++)
            {
                // If the first year is greater, swap the song details
                if(temp1->year > temp2->year)
                {
                    swap(temp1,temp2);
                }

                // Move to the next pair of adjacent nodes
                temp1=temp2;
                temp2=temp2->next;
            }
        }

        printf("\nSongs have been sorted");
    }

    writeData();       // Saves the sorted playlist
}


// ---------------- DISPLAY RECENTLY SEARCHED SONGS ----------------

// Displays all song names stored in the recent-search array
void display_recently_searched()
{
    int i;

    if(f==0)
    {
        printf("\nNo songs have been searched");
    }
    else
    {
        printf("\nRecently searched songs :");

        // Traverses the array and prints every searched song
        for(i=0; i<f; i++)
        {
            printf("\n%s",x.array[i].song);
        }
    }
}