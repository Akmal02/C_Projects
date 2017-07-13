#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

COORD coord;
HANDLE handle;

typedef char string[32];
typedef char longstring[140];

const int STATUS_AVAILABLE = 0;
const int STATUS_RESERVED = 1;
const int STATUS_IN_USE = 2;
const int STATUS_UNDER_MAINTENANCE = 3;

const int max_month[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void init()
{
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    system("mode 80, 30");
}

void gotoxy(int x, int y) {
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

void setcolor(int color) {
    SetConsoleTextAttribute(handle, color);
}


void cls()
{
    setcolor(0x0f);
    system("cls");
}

int numbers_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }

    return 1;
}

void create_banner(char* title, int color, int part)
{
    setcolor(0x08);
    gotoxy(60, 28);
    time_t t = time(NULL);
    struct tm current_time = *localtime(&t);
    char buff[60];
    strftime(buff, 60, "%d %b \4 %I:%M %p", &current_time);
    printf("%s", buff);

    gotoxy(0, 0);
    setcolor(color);
    printf("                                                                                ");
    printf("                                                                                ");
    printf("      %-72s  ", title);
    printf("                                                                                ");
    printf("                                                                                ");
    int i;
    for (i = 0; i < 5; i++)
    {
        if (i < part)
            printf("________________");
        else
            printf("                ");
    }
    setcolor(0x0f);
}

/* Print formatted hour. i.e. 1:23 pm */
void print_hour(time_t hour)
{
    struct tm time = *localtime(&hour);
    printf("%d:%02d %s", time.tm_hour - (time.tm_hour > 12 ? 12 : 0), time.tm_min, time.tm_hour < 12 ? "am" : "pm");
}

void print_date(time_t hour)
{
    struct tm time = *localtime(&hour);
    printf("%d/%d", time.tm_mday, time.tm_mon + 1);
}

/* Check if file exists */
/* Return 1 when so, 0 if not */
int fexists(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if(file != NULL) {
        // file exists
        fclose(file);
        return 1;
    }
    else return 0;
}


time_t current_time()
{
    time_t t = time(NULL);
    return t;
}

time_t set_time(int year, int month, int day, int hour, int minute)
{
    time_t t = current_time();
    struct tm *tm1 = localtime(&t);
    tm1->tm_year = year - 1900;
    tm1->tm_mon = month - 1;
    tm1->tm_mday = day;
    tm1->tm_hour = hour;
    tm1->tm_min = minute;
    tm1->tm_sec = 0;
    t = mktime(tm1);
    return t;
}

void scan_time(char *s, time_t *time)
{
    struct tm tm1;
    sscanf(s, "%d/%d/%d %d:%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year, &tm1.tm_hour, &tm1.tm_min);
    tm1.tm_year += 100;
    tm1.tm_mon -= 1;
    tm1.tm_sec = 0;
    *time = mktime(&tm1);
}

void fprint_time(FILE *f, time_t time)
{
    struct tm tm1 = *localtime(&time);
    char buff[60];
    strftime(buff, 60, "%d/%m/%y %H:%M", &tm1);
    fputs(buff, f);
}

void print_time(time_t time)
{
    fprint_time(stdout, time);
}

int diff_time(time_t t1, time_t t2)
{
    return (t2 - t1) / 60;
}



void get_time(time_t t, int *year, int *month, int *day, int *hour, int *minute)
{
    struct tm tm1 = *localtime(&t);
    *year = tm1.tm_year + 1900;
    *month = tm1.tm_mon + 1;
    *day = tm1.tm_mday;
    *hour = tm1.tm_hour;
    *minute = tm1.tm_min;
}

int is_same_day(time_t t1, time_t t2)
{
    int y, mo, d, h, m;
    int y2, mo2, d2, h2, m2;
    get_time(t1, &y, &mo, &d, &h, &m);
    get_time(t2, &y2, &mo2, &d2, &h2, &m2);
    return y == y2 && mo == mo2 && d == d2;
}

// get digit, 0 to 9, ignoring other characters
int getd()
{
    char c;
    do {
        c = getch();
    } while (!(c >= '0' && c <= '9'));
    putchar(c);
    return c - '0';
}

int show_error(char msg[60])
{
    system("color 0c");
    gotoxy(0, 20);
    setcolor(0xcf);
    printf("                                                                                ");
    printf("                                                                                ");
    printf("          %-60.60s          ", msg);
    printf("                                                                                ");
    printf("                                                                                ");
    Beep(1200, 200);
    Beep(1200, 200);
    
    getch();
}


void show_info(char msg1[60], char msg2[60], char msg3[60], char msg4[60])
{
    cls();
    setcolor(0x08);
    printf("\n\n");
    printf("    \333\333   \333\333   \333\333   \333\333\333\333\333\333   \333\333\333\333\333\333 \n");
    printf("    \333\333   \333\333\333  \333\333   \333\333      \333\333    \333\333\n");
    printf("    \333\333   \333\333 \333 \333\333   \333\333\333\333\333   \333\333    \333\333\n");
    printf("    \333\333   \333\333  \333\333\333   \333\333      \333\333    \333\333\n");
    printf("    \333\333   \333\333   \333\333   \333\333       \333\333\333\333\333\333 \n");
    int i, j;
    for (i = 0; i < 5; i++)
    {
    	gotoxy(70 + i * 2, i);
    	for (j = 0; j < (5 - i); j++)
    	{
    		printf(j % 2 == 0 ? "\260\260" : "  ");
		}
	}
    gotoxy(0, 13);
    setcolor(0x0f);
    printf("________________________________________________________________________________");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("     %-60.60s               ", msg1);
    printf("     %-60.60s               ", msg2);
    printf("     %-60.60s               ", msg3);
    printf("     %-60.60s               ", msg4);
    printf("                                                                                ");
    setcolor(0x0e);
    printf("                                               Press any key to continue  >>    ");
    setcolor(0x0f);
    printf("________________________________________________________________________________");
    getch();
}

void help_start()
{
    cls();
    setcolor(0x03);
    printf("\n\n");
    printf("    \333\333   \333\333  \333\333\333\333\333\333  \333\333       \333\333\333\333\333\n");
    printf("    \333\333   \333\333  \333\333      \333\333       \333\333   \333\333\n");
    printf("    \333\333\333\333\333\333\333  \333\333\333\333\333   \333\333       \333\333   \333\333\n");
    printf("    \333\333   \333\333  \333\333      \333\333       \333\333\333\333\333\n");
    printf("    \333\333   \333\333  \333\333\333\333\333\333  \333\333\333\333\333\333\333  \333\333\n");
    
 
    gotoxy(0, 13);
    setcolor(0x0b);
    printf("________________________________________________________________________________");
    printf("                                                                                ");
    printf("                                                                                ");
}

int verify_credit(char num[16])
{
	const static int doubled[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};
	int i;
	int sum = 0;
	
	for (i = 0; i < 16; i++)
	{
		if (i % 2 == 0)
			sum += doubled[num[i] - '0'];
		else
			sum += num[i] - '0';

	}
	// Credit card is valid is sum is divisible by 10

	return sum % 10 == 0;
}

void help_end()
{
    getch();
}


// END OF FILE car_rental.h
