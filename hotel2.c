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


/*  Printing involves three functions:
 *  (This is much fancier than printf() with lots of \n's)
 * 
 *    - setcolor() changes the current pen color
 *    - gotoxy() move the pen to the desired coordinate
 *    - printf() starts writing to the screen
 *  The process continues...
 */

/* Note: for more information about colors you can go to Command Prompt and type "color h". Respective color code will come out.
 * TODO: We'll make it easier next time. Don't worry.
 * For now, just try and enjoy :D
 */

// Required variables for certain Windows function.
COORD coord;
HANDLE handle;

// Each hotel will have...
struct HOTEL
{
	char name[32];				// A hotel name, of course
	int star;					// How many star the hotel deserves
	int rating;					// User rating
	int price;					// Price per night
};

// This is a list of hotels we have...
struct HOTEL hotels[] = {
	{"Pavillon Hotel", 5, 5, 180},
	{"Angsana Hotel", 4, 4, 165},
	{"Super Hotel", 4, 4, 130},
	{"Macha Port", 2, 5, 85},
	{"Legend Hotel", 4, 4, 120},
	{"Wann Hotel", 3, 4, 115},
	{"Kasanova Hotel", 5, 5, 210},
	{"Vida Hotel", 6, 6, 400}
  // And more here...
};

// String representation of the stars
char stars[7][12] = {
	"           ",
	"*          ",
	"* *        ",
	"* * *      ",
	"* * * *    ",
	"* * * * *  ",
	"* * * * * *"
};

// For the hearts too (\3 means heart)
char heart[7][12] = {
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
	setcolor(0x0f);
	gotoxy(20, 5);
	printf("+--------------------------------------+");
	gotoxy(20, 6);
	printf("|                                      |");
	gotoxy(20, 7);
	printf("+--------------------------------------+");
}


void print_title_bar()
{
	setcolor(0x06);
	gotoxy(47, 1);
	printf("%30.30s", "Welcome, Guest!");
	gotoxy(68, 2);
	printf("[L] Login");

	gotoxy(3, 1);
	setcolor(0x0f);
	disptime();

	setcolor(0x0f);
	gotoxy(0, 3);
	printf("-------------------------------------------------------------------------------\n");
	gotoxy(0, 27);
	printf("-------------------------------------------------------------------------------\n");

	setcolor(0x0e);
	gotoxy(3, 28);
	printf("[H] Help     [A] About");


	setcolor(0x08);
	gotoxy(47, 28);
	printf("%30s", "(C) Syahrulnizam Syaz. Inc");

}


void find_hotel()
{
	do
	{
		system("color 80");
		setcolor(0x0f);
		print_search_box();
		gotoxy(23, 6);
		printf("%c   ", ARROWR);
		char srchtext[32];
		scanf("%s", &srchtext);

		system("cls");
		print_title_bar();
		print_search_box();
		gotoxy(22, 6);
		printf(" *   ");
		printf("%s", srchtext);
		setcolor(0x0f);
		gotoxy(6, 9);
		printf("Search resullts...\n");
		gotoxy(6, 11);
		printf("      Name                 Star         Rating         Price\n");
		printf("   -----------------------------------------------------------------------\n");
		for (i = 0; i < 8; i++)
		{
			setcolor(0x0f);
			printf("      %2d", i + 1);
			printf("  %-15.15s", hotels[i].name);
			setcolor(0x0e);
			printf("     %s", stars[hotels[i].star]);
			setcolor(0x0c);
			printf("   %s", heart[hotels[i].rating]);
			setcolor(0x0f);
			printf("   RM %4d.00\n", hotels[i].price);
		}
		c = getch();
	}
	while (c == '\r');
}



// =================================================================================================
// ======================== MAIN FUNCTION
// ====================================================
// =================================================================================================

int main()
{
	// Needed for setcolor and gotoxy functions
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	char srchtext[32];

  // Limit the window size to 80x30
	system("mode 80, 30");

	while (1)
	{
		system("cls");
		print_title_bar();
		print_search_box();

		setcolor(0x0f);
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


		gotoxy(23, 6);
		printf("%c   Find Hotel", ARROWR);

		// Wait for next keyboard press 
		c = getch();

		switch (c)
		{
		case 'l':				/* Login */
			system("cls");
			break;

		case '\r':			/* Find hotel */
			find_hotel();
			break;

		case 'h':				/* Help */
			system("cls");
			break;

		case 'a':				/* Abuot */
			system("cls");
			break;
		}
	}

	system("cls");
	system("pause");
	return 0;
}
