/*
 *     HOTEL RESEEVATION SYSTEM
 *     © 2016
 *
 */




#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define ARROWR 26
#define ARROWL 27

#define GREY 8
#define BLUE 9
#define GREEN 10
#define AQUA 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15


/*  Printing involves three functions:
 *  (This is much fancier than printf() with lots of \n's)
 * 
 *    - setcolor() changes the current pen color
 *    - gotoxy() move the pen to the desired coordinate
 *    - printf() starts writing to the screen
 *  The process continues...
 *  "pen" here means the blinking cursor you see in the output. Nothing strange.
 */

/* Note: for more information about colors you can go to Command Prompt 
 * and type "color h". Respective color code will come out.
 * TODO: We'll make it easier next time. Don't worry.
 * For now, just try and enjoy :D
 */

// Required variables for certain Windows function.
COORD coord;
HANDLE handle;

// Each hotel will have...
typedef struct
{
	char name[32];				// A hotel name, of course
    char location[40];          // Where is the hotel
	int star;					// How many star the hotel deserves
	int rating;					// User rating
	int single_price;			// Price per night
	int double_price;           // same as above, but for double room           
	int weekend_disc;           // discount when on weekend
	char facilities[6];         // w = wifi, p = pool, s = spa, b = beach
	int hits;                    // Popularity (based on number of customers)
} HOTEL;
// A new data type: HOTEL!


// This is a list of hotels we have...
const HOTEL hotels[] = {
	{"Pavillon Hotel", "Kuala Lumpur", 5, 5, 180},
	{"Angsana Hotel", "Puncak Alam, Selangor", 4, 4, 165},
	{"Super Hotel", "Batu Pahat, Johor", 4, 4, 130},
	{"Macha Port", "Port Dickson, N. Sembilan", 2, 5, 85},
	{"Legend Hotel", "Pasir Puteh, Kelantan", 4, 4, 120},
	{"Wann Hotel", "Jitra, Kedah", 3, 4, 115},
	{"Kasanova Hotel", "Besut, Terengganu", 5, 5, 210},
	{"Vida Hotel", "Kota Bharu, Kelantan", 6, 6, 400}
  // And more here...
};

HOTEL hotel_search_results[30];

// String representation of the stars
const char stars[7][12] = {
	"           ",
	"*          ",
	"* *        ",
	"* * *      ",
	"* * * *    ",
	"* * * * *  ",
	"* * * * * *"
};

// For the hearts too (\3 means heart)
const char heart[7][12] = {
	"           ",
	"\3          ",
	"\3 \3        ",
	"\3 \3 \3      ",
	"\3 \3 \3 \3    ",
	"\3 \3 \3 \3 \3  ",
	"\3 \3 \3 \3 \3 \3"
};


// =======  FUNCTION DECLARATION =======


// Move cursor to the desired coordinate (x, y)
// where (0, 0) is the top left corner
// Like a graph paper, but with inverted y-axis
//   (0, 0)  *--------------------------------------*  (80, 0)
//           |  Output here.                        |
//           |  Press any key to continue._         |
//           |                                      |
//           |                                      |
//           |                                      |
//           |                                      |
//   (0, 30) *--------------------------------------*  (80, 30)

void gotoxy(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

// Change the color for the next printf
void setcolor(int color)
{
	SetConsoleTextAttribute(handle, color);
}

// ==============================================================

char c;
int i;


void disptime()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%s", asctime(&tm));

}

void print_search_box()
{
	setcolor(WHITE);
	gotoxy(15, 5);
	printf("+--------------------------------------+");
	gotoxy(15, 6);
	printf("|                                      |");
	gotoxy(15, 7);
	printf("+--------------------------------------+");
}


void print_title_bar()
{
	setcolor(YELLOW);
	gotoxy(47, 1);
	printf("%30.30s", "Welcome, Guest!");
	gotoxy(68, 2);
	printf("[L] Login");

	gotoxy(3, 1);
	setcolor(WHITE);
	disptime();

	setcolor(WHITE);
	gotoxy(0, 3);
	printf("-------------------------------------------------------------------------------\n");
	gotoxy(0, 27);
	printf("-------------------------------------------------------------------------------\n");

	setcolor(WHITE);
	gotoxy(3, 28);
	printf("[H] Help     [A] About");


	setcolor(GREY);
	gotoxy(47, 28);
	printf("%30s", "(C) Syahrulnizam Syaz. Inc");

}


void screen_home();
void screen_login();
void screen_find_hotel();
void screen_hotel_select(HOTEL hotel);
void screen_help();
void screen_about();


void process_for_result(char* srchtext);

// =================================================================================================
// ======================== MAIN FUNCTION
// ====================================================
// =================================================================================================


int main()
{
	// Needed for setcolor and gotoxy functions
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Limit the window size to 80x30
	system("mode 80, 30");

	screen_home();
	return 0;
}


void screen_home() 
{
	
	char srchtext[32];
	
	while (1)
	{
		system("cls");
		print_title_bar();
		print_search_box();

		setcolor(WHITE);
		gotoxy(0, 13);
		printf
			("      _    _                                                                   \n");
		printf
			("     | |__| |                                                                  \n");
		printf
			("     |  __  |                                                                  \n");
		printf
			("     | |  | |                                                                  \n");
		printf
			("     |_|  |_|                                                                  \n");


		gotoxy(19, 6);
		printf("%c   Press [Enter] to search", ARROWR);

		// Wait for next keyboard press 
		c = getch();

		switch (c)
		{
			case 'l':				/* Login */
				system("cls");
				screen_login();
				break;
				
			case '\r':			/* Find hotel */
				screen_find_hotel();
				break;

			case 'h':				/* Help */
				system("cls");
				screen_help();
				break;

			case 'a':				/* About */
				system("cls");
				screen_about();
				break;
		}
	}

	system("cls");
	system("pause");
}




void screen_login()
{
 	 
}



void screen_find_hotel()
{ 	 
	do
	{
		system("color 80");
		setcolor(WHITE);
		print_search_box();
		gotoxy(19, 6);
		printf("%c   ", ARROWR);
		char srchtext[32];
		scanf("%s", &srchtext);

		system("cls");
		print_title_bar();
		print_search_box();
		
		setcolor(WHITE);
		gotoxy(57, 5);
		printf("+------------+");
		gotoxy(57, 6);
		printf("|            |");
		gotoxy(57, 7);
		printf("+------------+");
		
		gotoxy(19, 6);
		printf("*   ");
		printf("%s", srchtext);
		setcolor(WHITE);

		gotoxy(6, 9);
	
	
		printf("Search resullts...\n");
	
	    process_for_result(srchtext);
	    
		gotoxy(6, 12);
		printf("      Name                 Star         Rating         Price\n");
		printf("   -----------------------------------------------------------------------\n");
		
		for (i = 0; i < 8; i++)
		{
			setcolor(GREY);
			printf("      %2d", i + 1);
			setcolor(0x0f);
			printf("  %-15.15s", hotel_search_results[i].name);
			setcolor(0x0e);
			printf("     %s", stars[hotel_search_results[i].star]);
			setcolor(0x0c);
			printf("   %s", heart[hotel_search_results[i].rating]);
			setcolor(0x0f);
			printf("   RM %4d.00\n", hotel_search_results[i].single_price);
		}
		c = getch();
		
		switch (c) {
            case '1':  case '2':  case '3':  case '4':
            case '5':  case '6':  case '7':  case '8':
				 screen_hotel_select(hotel_search_results[c - '0' - 1]);
	        break;
  		}
	}
	while (c == '\r');
}



void screen_hotel_select(HOTEL hotel)
{
     system("cls");
     print_title_bar();
     setcolor(GREY);
     gotoxy(6, 6);
     printf("+----------------------------------------------------------------+");
     for (i = 7; i < 13; i++)
     {
	  	 gotoxy(6, i);    printf("|");
	  	 gotoxy(71, i);   printf("|");
	 }
     gotoxy(6, 12);
     printf("+----------------------------------------------------------------+");

     gotoxy(10, 8);
     setcolor(WHITE);
     printf(hotel.name);

     gotoxy(10, 10);
     setcolor(GREY);
     printf(hotel.location);

     gotoxy(37, 8);
     setcolor(YELLOW);
     printf(stars[hotel.star]);

     gotoxy(37, 10);
     printf("%d stars", hotel.star);

     gotoxy(54, 8);
     setcolor(RED);
     printf(heart[hotel.rating]);
     printf("5.0");

     gotoxy(54, 10);
     setcolor(PURPLE);
     printf("from 2000 users");
     
     getch();
}


void process_for_result(char* srchtext)
{
 	 for (i = 0; i < 5; i++)
         hotel_search_results[i] = hotels[i]; 	 
}
	    
void screen_help()
{
 	 
}


void screen_about()
{
 	 
}


/* =====   END OF FILE hotel2.c   ===== */
