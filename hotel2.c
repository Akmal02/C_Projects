/* 
 *     HOTEL RESERVATION SYSTEM
 *     � 2016
 *
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <shlwapi.h>

#define BACKSPACE 8
#define ENTER 13

#define GREY 8
#define BLUE 9
#define GREEN 10
#define AQUA 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

#define HOTEL_MAX_SIZE 50

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

// Multipurpose variable
char c;
int i;

// Printing involves three functions:
// (This is much fancier than printf() with lots of \n's)
// - setcolor() changes the current pen color
// - gotoxy() move the pen to the desired coordinate
// - printf() starts writing to the screen
// ...and the process continues...  
// "pen" here means the blinking cursor you see in the output. 
// Nothing strange.

// Each hotel will have...
typedef struct
{
	char name[32];				// A hotel name, of course
	char location[40];			// Where is the hotel
	int star;					// How many star the hotel deserves
	float rating;				// User rating
	int hits;					// Popularity (based on number of reviews
	int single_price;			// Price per night
	int double_price;			// same as above, but for double room 
	float weekend_disc;			// discount when on weekend
	char facilities[6];			// w = wifi, p = pool, s = spa, b = beach
} HOTEL;
// A new data type: HOTEL!


// This is a list of hotels we have...
const HOTEL hotels[HOTEL_MAX_SIZE] = {
	{"Pavillon Hotel", "Kuala Lumpur", 5, 4.5, 1320, 180, 350, 0.1, "wps"},
	{"Angsana Hotel", "Puncak Alam, Selangor", 2, 4.6, 270, 65, 100, 0.0, "w"},
	{"Super Hotel", "Batu Pahat, Johor", 3, 4.4, 3000, 130, 260, 0.2, "wps"},
	{"Macha Port", "Port Dickson, N. Sembilan", 4, 4.8, 850, 150, 280, 0.25, "wpsb"},
	{"Legend Hotel", "Pasir Puteh, Kelantan", 4, 4.2, 1200, 120, 230, 0.1, "ws"},
	{"Wann Hotel", "Jitra, Kedah", 3, 4.5, 2300, 100, 190, 0.1, "wpb"},
	{"Kasanova Hotel", "Besut, Terengganu", 5, 4.7, 500, 230, 400, 0.15, "wpsb"},
	{"Vida Hotel", "Kota Bharu, Kelantan", 6, 5.0, 400, 290, 450, 0.35, "wpsb"}
	// And more here...
};

static const HOTEL empty_hotel;

// Temporary array to store the search result.
HOTEL hotel_search_results[HOTEL_MAX_SIZE];

char current_user[32] = "";
char srchtext[32];

// Required variables for certain Windows function.
COORD coord;
HANDLE handle;

// ======= UTILITY FUNCTION =======


// Move cursor to the desired coordinate (x, y)
// where (0, 0) is the top left corner and (80, 300 is the bottom-right.
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

// Function to determine the day of week given date
// Output will be an integer between 0 and 6
// where 0 = Sunday, 1 = Monday, 2 = Tuesday ... 6 = Saturday
// (From https://en.m.wikipedia.org/wiki/Determination_of_the_day_of_the_week)
// 
// Usage: dayofweek(20, 2, 2016) will return 6 [Saturday]

int dayofweek(int d, int m, int y)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}



void disptime()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%s", asctime(&tm));
}


// =============================================================

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

	gotoxy(3, 1);
	setcolor(WHITE);
	disptime();

	setcolor(WHITE);
	gotoxy(0, 3);
	printf("-------------------------------------------------------------------------------\n");
	gotoxy(0, 27);
	printf("-------------------------------------------------------------------------------\n");

	if (strcmp(current_user, "") == 0)
	{
		setcolor(YELLOW);
		gotoxy(47, 1);
		printf("%30.30s", "Welcome, Guest!");
		gotoxy(68, 2);
		printf("[L] Login");
	}
	else
	{
		setcolor(GREEN);
		gotoxy(47, 1);
		char temp[32];
		sprintf(temp, "Welcome, %s!", current_user);
		printf("%30.30s", temp);
		gotoxy(69, 2);
		printf("[L] More");
	}
}


void print_tips(char *tips)
{
	setcolor(WHITE);
	gotoxy(3, 28);
	printf(tips);
}

void screen_home();
void screen_login();
void screen_find_hotel();
void screen_filter();
void screen_hotel_select(HOTEL hotel);
void screen_booking_hotel();
void screen_help();
void screen_about();

void show_searching();
int process_for_result(char *srchtext);

// =================================================================================================
// ============================== MAIN FUNCTION
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
		printf("%c   Press [Enter] to search", 27);


		print_tips("[H] Help     [A] About");


		setcolor(GREY);
		gotoxy(47, 28);
		printf("%30s", "(C) Syahrulnizam Syaz. Inc");


		// Wait for next keyboard press 
		c = getch();

		switch (c)
		{
		case 'l':				/* Login */
			system("cls");
			screen_login();
			break;

		case ENTER:			/* Find hotel */
			show_searching();
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
	strcpy(current_user, "Akmal");
}


void show_searching()
{
	system("color 80");
	setcolor(WHITE);
	print_search_box();

	gotoxy(19, 6);
	printf("%c   ", 27);
	scanf("%s", &srchtext);
}

void screen_find_hotel()
{
	while (1)
	{

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

		int found = process_for_result(srchtext);

		if (found != 0)
		{
			gotoxy(6, 12);
			printf("      Name                 Star         Rating         Price\n");
			printf("   -----------------------------------------------------------------------\n");

			HOTEL hotel;
			for (i = 0; i < found; i++)
			{
				hotel = hotel_search_results[i];

				setcolor(GREY);
				printf("      %2d", i + 1);
				setcolor(0x0f);
				printf("  %-15.15s", hotel.name);
				setcolor(0x0e);
				printf("     %s", stars[hotel.star]);
				setcolor(0x0c);
				printf("   %s", heart[(int)hotel.rating]);
				setcolor(0x0f);
				printf("   RM %4d.00\n", hotel.single_price);
			}

			print_tips("Press a number to select the hotel, [F] to filter and sort result");
		}
		else
		{
			setcolor(GREY);
			gotoxy(0, 14);
			printf("             ______          No hotel found.            \n");
			printf("            | _  _ |          Too bad...                \n");
			printf("            |      |                                    \n");
			printf("            |  __  |                                    \n");
			printf("            \\______/                                    \n");
			print_tips("Press [Enter] to search again");
		}

		c = getch();

		switch (c)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			screen_hotel_select(hotel_search_results[c - '0' - 1]);
			break;
		case 'l':
			screen_login();
			break;
		case ENTER:
			show_searching();
			break;
		case BACKSPACE:
			return;
		}

	}
}

void screen_filter()
{



}


void screen_hotel_select(HOTEL hotel)
{
	while (1)
	{
		if (strcmp(hotel.name, "") == 0)
			return;
		system("cls");
		print_title_bar();
		setcolor(GREY);
		gotoxy(6, 6);
		printf("+----------------------------------------------------------------+");
		for (i = 7; i < 13; i++)
		{
			gotoxy(6, i);
			printf("|");
			gotoxy(71, i);
			printf("|");
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
		printf("%d star", hotel.star);
		if (hotel.star > 1)
			printf("s");

		gotoxy(54, 8);
		setcolor(RED);
		printf("%.1f  ", hotel.rating);
		printf(heart[(int)hotel.rating]);

		gotoxy(51, 10);
		setcolor(PURPLE);
		printf("   %5d reviews", hotel.hits);

		setcolor(GREY);
		gotoxy(0, 13);

		printf("      |                                                                |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |   from...              |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |           per night    |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                       |                        |\n");
		printf("      |                                                                |\n");
		printf("      +----------------------------------------------------------------+\n");

		setcolor(WHITE);
		gotoxy(52, 17);
		printf(" RM %3d.00", hotel.single_price);

		setcolor(GREEN);
		gotoxy(50, 22);
		printf("  Press [Enter]  ");
		gotoxy(50, 23);
		printf("  to proceed    >>");


		print_tips("Press [Backspace] to cancel");
		c = getch();

		switch (c)
		{
		case 'l':
			screen_login();
			break;
		case ENTER:
			screen_booking_hotel();
			break;
		case BACKSPACE:
			return;
		}
	}

}


int process_for_result(char *srchtext)
{
	int found = 0;
	HOTEL hotel;


	for (i = 0; i < 8; i++)
	{
		// Clear out existing search result
		hotel_search_results[i] = empty_hotel;

		hotel = hotels[i];
		if (StrStrI(hotel.name, srchtext) != 0 || StrStrI(hotel.location, srchtext) != 0)
		{
			hotel_search_results[found++] = hotel;
		}
	}

	return found;
}

void screen_booking_hotel()
{
	while (1)
	{
		system("cls");
		print_title_bar();

		getch();
		break;
	}
}

void screen_help()
{

}


void screen_about()
{

}


/* ===== END OF FILE hotel2.c ===== */ 
