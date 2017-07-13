#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "car_rental.h"

int reserve(char name[30], int car, time_t when, int duration);
int load_data();
int main();
void admin_mode();
void info(char msg1[60], char msg2[60], char msg3[60], char msg4[60]);
void loading(char text[50], int delay);
void add_user(char tn[50], char tp[50], char ti[50]);
void add_to_record(char user[30], int car, double price);
void exiting();
void first_time_check();
void get_promotions();
void load_user();
void make_payment();
void price_table();
void print_car_table();
void print_receipt();
void save_data();
void save_user();
void select_car();
void select_location();
void select_time();
void title();
void update_data();
void user_details();


char places[6][20], car_in_place[6];
int toggle_help = 1;

char car_model[20][32];
char car_plate[20][32];
int car_location[20]; // 1 = UTM, 2 = Taman U
char car_size[20]; // S = Small, L = Large
char car_automanual[20]; // A = Auto, M = Manual
int car_status[20]; // 1 = Available, 2 = Reserved, 3 = In use, 4 = Maintenance
int car_duration[20];
time_t car_time[20];
int number_of_cars;

int sel_location;
int sel_car;
time_t sel_time;
int sel_duration;

char name[50][30], phone[50][30], ic[50][30];
int usage[30];
int num_user = 0;

char sel_name[30];
char phoneno[30];
int promotions = 0;


double payment;
double user_pay, total_payment;
int pay_via_creditcard = 0;

// Small Auto
const float rate1 = 10,
			rate1a = 55,
            rate1b = 108,
            rate1c = 216,
            rate1d = 1512;
// Small Manual
const float rate2 = 8,
			rate2a = 43,
            rate2b = 86,
            rate2c = 173,
            rate2d = 1210;

// Large Auto
const float rate3 = 15,
			rate3a = 80,
            rate3b = 150,
            rate3c = 270,
            rate3d = 1400;
// Large Manual
const float rate4 = 12,
			rate4a = 65,
            rate4b = 110,
            rate4c = 200,
            rate4d = 1320;


// Main entry for the program
int main() {

    init();
    first_time_check();
	load_data();    
    info("Welcome to Car Rental System!", "This infobox will guide you to operate this program.","You may turn this infobox on/off by pressing [H] on the", "title screen.");
    loading("Loading...", 60);
    title();
    select_location();
    loading("Searching for cars...", 10);
    select_car();
    price_table();
    select_time();
    user_details();
    make_payment();
    reserve(sel_name, sel_car, sel_time, sel_duration);
    exiting();
}

/* UI module */

// Simulate a loading screen
void loading(char text[50], int delay) {
    srand(time(NULL));
    cls();
    gotoxy(3, 24);
    printf(text);

    gotoxy(0, 26);
    int i;
    for (i = 0; i < 80; i+=2)
    {
    	gotoxy(i, 26);
        printf("__");
        gotoxy(72, 24);
        printf("%3d %%", (int) (1.25 * i)); // 80/80*100 == 80*1.25
        Sleep(rand() % 10 > 3 ? 5 : rand() % 10 < 3 ? delay * 3 : delay); // For variety. 30% probability that it (will) skip a part, or triple the loading delay
    }
}

// Show infobox 
void info(char msg1[60], char msg2[60], char msg3[60], char msg4[60]) {
    if (toggle_help)
    {
        cls();
        show_info(msg1, msg2, msg3, msg4);
    }
}

// A title screen
void title() {
    cls();
    setcolor(0x0f);
    printf("\n\n      Welcome to...\n\n");
    setcolor(0x0e);
    printf("            ______              ____             __        __   \n");
    printf("           / ____/___ ______   / __ \\___  ____  / /_____ _/ /  \n");
    printf("          / /   / __ `/ ___/  / /_/ / _ \\/ __ \\/ __/ __ `/ /  \n");
    setcolor(0x0c);
    printf("         / /___/ /_/ / /     / _, _/  __/ / / / /_/ /_/ / /     \n");
    printf("         \\____/\\__,_/_/     /_/ |_|\\___/_/ /_/\\__/\\__,_/_/ \n");

    setcolor(0x0e);
    printf("                 _____            __                              \n");
    printf("                / ___/__  _______/ /____  ____ ___                \n");
    printf("                \\__ \\/ / / / ___/ __/ _ \\/ __ `__ \\           \n");
    setcolor(0x0c);
    printf("               ___/ / /_/ (__  ) /_/  __/ / / / / /               \n");
    printf("              /____/\\__, /____/\\__/\\___/_/ /_/ /_/             \n");
    printf("                   /____/                                         \n");
    setcolor(0x0f);
    printf("\n\n                        Press any key to continue");
    setcolor(0x07);
    gotoxy(3, 28);
    printf("Press [H]: toggle infobox");

    char c = getch();
    if (c == ' ')
        admin_mode();
    else if (c == 'H' || c == 'h')
    {
        if (toggle_help == 0)
        {
            toggle_help = 1;
            info("You have turn on infobox.", "This box will guide you to use this program, ", "as usual. Perhaps you know how to turn it back off? ;)", "Anyway, welcome back!");
        } else if (toggle_help == 1)
        {
            info("You have turn off infobox.", "Seems like you know how to use this program ;)", "You can always turn it back on by pressing [H]", "next time you restart the program.");
            toggle_help = 0;
        }
    }

}

// Called on first time launch... Loading initial files etc.
void first_time_check() {
	
    if (!fexists("first"))
    {
        toggle_help = 1;
        FILE *f = fopen("first", "w");
        if (f == NULL)
        {
            info("For some reason, this program failed to work... :(", "The filesystem you are using are not writable.", "You may want to change location by copying this program", "and its data to a writable loction.");
            exit(1);
        }
        fclose(f);
    } else {
        toggle_help = 0;
    }
    
    if (!fexists("cars.txt"))
    {
    	// Test file
    	loading("Preparing file...", 20);
    	FILE *f = fopen("cars.txt", "w");
    	fprintf(f, "5 cars in UTM\n");
		fprintf(f, "* WJB1234, Perodua MyVi, LA, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* JDT0192, Perodua Kancil, SM, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* JHB3243, Proton Saga, SM, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* WHB7730, Perodua Viva, LA, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* MCQ6261, Perodua Bezza, LA, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "5 cars in Taman U\n");
		fprintf(f, "* JQB4601, Perodua Kancil, SM, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* JDF3491, Perodua MyVi, LM, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* WMA2301, Proton Wira, SM, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* MAT1110, Perodua Kancil, SA, 1, 0, 01/01/10 00:00\n");
		fprintf(f, "* JGL3932, Perodua Viva, LA, 1, 0, 01/01/10 00:00\n");
		fclose(f);
	}

}


/* Admin module (Unfinished) */

// launches admin mode
void admin_mode() {
    show_info("You have entered administrator mode.", "This mode are meant to be used by admin to take in/out the",
         "car or to view all car's status", "(How do you manage to find this?)");
    int car;
    while (1) {
        cls();
        printf("\n\n\n");
        print_car_table();
        setcolor(0x07);

        printf("\n\n   Admin mode\n");
        printf("   Select a car... ");
        scanf("%d", & car);
        if (car >= 1 && car <= number_of_cars) {
            car--;
            setcolor(0x0f);
            printf("\n\n");
            if (car_status[car] == 1)
                printf("   1. Put the car in maintenance");
            else if (car_status[car] == 4)
                printf("   1. Make available");
            else
                printf("   1. Put the car in maintenance (Slower)");

            char c = getch();
            if (c == '1') {
                if (car_status[car] == 1) {
                    car_status[car] = 4;
                } else if (car_status[car] == 4) {
                    car_status[car] = 1;
                } else {
                    loading("Forcing the user to return the car... :P", 40);
                    car_status[car] = 1;
                }
                save_data();
            }
        }

    }
}

// Print the table of cars available with its details and booking status
void print_car_table() {
    setcolor(0x07);
    printf("  No.  RegNo     Car            Location  Type  Status\n\n");
    int i;
    for (i = 0; i < number_of_cars; i++) {

        setcolor(0x07);
        printf("  %2d. ", i + 1);
        setcolor(0x0f);
        printf("[%s] %-15.15s ", car_plate[i], car_model[i]);
        setcolor(0x0e);
        printf("%-8.8s", places[car_location[i]]);
        setcolor(0x07);
        printf("  %c%c    ", car_size[i], car_automanual[i]);
        switch (car_status[i]) {
        case 1:
            setcolor(0x0a); /* Green */
            printf("Available");
            break;
        case 2:
            setcolor(0x0e); /* Yellow */
            printf("Reserved ");
            printf("at ");
            if (!is_same_day(current_time(), car_time[i])) {
                print_date(car_time[i]);
                printf(", ");
            }
            print_hour(car_time[i]);
            break;
        case 3:
            setcolor(0x0c); /* Red */
            printf("In use ");
            printf("until ");
            if (!is_same_day(current_time(), car_time[i])) {
                print_date(car_time[i]);
                printf(", ");
            }
            print_hour(car_time[i]);
            break;
        case 4:
            setcolor(0x07); /* Gray */
            printf("Maintenance");
            break;

        }
        printf("\n");
    }
}


/* Storage module */  
int load_data() {
    FILE * data = fopen("cars.txt", "r");

    if (data == NULL) {
        info("Sorry but, an error has occured", "\"cars.txt\" file are either gone missing or", "is unaccessible. The program will exit now.", "Sorry for the inconvienence");
        exit(1);
    }

    int i = 0, n = 0, p = 0;
    char place[20];
    int dummy;
    char buff[20];

    while (!feof(data)) {
        if (n == 0) {
            fscanf(data, "%d cars in %[^\n]\n", & n, & place);
            p++;
            strcpy(places[p], place);
            car_in_place[p] = n;
            number_of_cars += n;
        }
        fscanf(data, "* %[^,], %[^,], %c%c, %d, %d, %[^\n]\n", & car_plate[i], & car_model[i], & car_size[i], & car_automanual[i], & car_status[i], & car_duration[i], & buff);
        scan_time(buff, & car_time[i]);
        car_location[i] = p;
        i++;
        n--;
    }

    fclose(data);
    update_data();
    return 0;
}

void update_data() {
    int i;
    time_t now = current_time();
    for (i = 0; i < number_of_cars; i++) {
        switch (car_status[i]) {
        case 1: // Available
            break;
        case 2: // Reserved
            if (now > car_time[i]) {
                car_status[i] = 3;
                car_time[i] += car_duration[i] * 60;
            }
            break;
        case 3:
            if (now > car_time[i]) {
                car_status[i] = 1;
                car_time[i] = 0;
                car_duration[i] = 0;
            }
            break;
        case 4:
            break;
        }
    }
}

void save_data() {
    FILE * data = fopen("cars.txt", "w");

    int i = 0, n = 0, p = 0;

    while (1) {
        if (n == 0) {
            p++;
            if (car_in_place[p] <= 0) break;

            fprintf(data, "%d cars in %s\n", car_in_place[p], places[p]);
            n = car_in_place[p];
        }
        fprintf(data, "* %s, %s, %c%c, %d, %d, ", car_plate[i], car_model[i],
                car_size[i], car_automanual[i], car_status[i], car_duration[i]);
        fprint_time(data, car_time[i]);
        fputc('\n', data);
        i++, n--;

    }
    fclose(data);
    return;
}

int reserve(char name[30], int car, time_t when, int duration) {
    car_status[car] = 2;
    car_time[car] = when;
    car_duration[car] = duration;
    add_to_record(name, car, payment);
    save_data();
    return 1;
}

void add_to_record(char user[30], int car, double price) {
    int first_time = !fexists("records.txt");
    FILE * record = fopen("records.txt", "a");

    if (first_time) {
        fprintf(record, "Booking History\n");
        fprintf(record, "------------------------------------------------------------------------------\n");
        fprintf(record, "DATE     TIME  | USER       CAR                REGNO      DURATION     PRICE  \n");
        fprintf(record, "------------------------------------------------------------------------------\n");
    }

    fprint_time(record, car_time[car]);
    fprintf(record, " | %-10.10s %-18.18s %-10.10s %d hours %10.2f\n", user, car_model[car], car_plate[car], car_duration[car], price);
    fclose(record);

}


/* Selection module */
void select_location() {

    info("Select a location for car pickup.", "You may use the UP/DOWN button to navigate between", "the available options and press ENTER to select it.", "");
    int selected = 1;
    char c;
    while (1) {
        cls();
        create_banner("Select location", 0x2e, 1);
        gotoxy(14, 9);
        printf("SELECT YOUR LOCATION:\n\n");
        gotoxy(11, 12);
        printf("%c *TAMAN UNIVERSITI\n", selected == 1 ? '>' : ' ');
        gotoxy(11, 14);
        printf("%c *INSIDE UTM\n", selected == 2 ? '>' : ' ');
        gotoxy(11, 16);
        printf("%c *Exit\n", selected == 3 ? '>' : ' ');
        c = getch();
        if (c == -32) c = getch(); // Reduce flicker
        switch (c) {
        case 72:
            selected--;
            break; // UP
        case 80:
            selected++;
            break; // DOWN
        case 13: // ENTER
            if (selected == 1 || selected == 2)
                sel_location = selected;
            else
            {
                exiting();
            }
            return;
        }
        if (selected > 3) selected = 3;
        if (selected < 1) selected = 1;
    }
}

void select_car() {

    info("Select an available car.", "You can select an available car depending on your choice.", "Some of the cars might not be available at the moment.", "");

    cls();
    int i, s = 0, p = 0;
    char c;

    if (sel_location == 2) s = 5;
    while (1) {
        cls();
        create_banner("Select car", 0x4f, 2);
        p = 0;
        gotoxy(3, 9);
        setcolor(0x07);
        printf("    RegNo.     Car           Size      Trans.  Status");
        for (i = 0; i < 10; i++) {
            if (car_location[i] == sel_location) {
                setcolor(0x0f);
                gotoxy(3, 12 + p++ * 2);
                printf(" %c ", s == i ? '>' : ' ');
                printf(" %s ", car_plate[i]);
                printf(" %-15s ", car_model[i]);
                printf("%c %s", car_size[i], car_size[i] == 'L' ? "\334\333\333\333\334  " : "\334\333\333\334   ");
                printf(" %s ", car_automanual[i] == 'A' ? "Auto   " : "Manual ");
                switch (car_status[i]) {
                case 1:
                    setcolor(0x0a);
                    printf("AVAILABLE");
                    break;
                case 2:
                    setcolor(0x0e);
                    printf("RESERVED ");
                    printf("AT ");
                    if (!is_same_day(current_time(), car_time[i])) {
                        print_date(car_time[i]);
                        printf(", ");
                    }
                    print_hour(car_time[i]);
                    break;
                case 3:
                    setcolor(0x0c);
                    printf("IN USE");
                    printf(" UNTIL ");
                    if (!is_same_day(current_time(), car_time[i])) {
                        print_date(car_time[i]);
                        printf(", ");
                    }
                    print_hour(car_time[i]);
                    break;
                case 4:
                    setcolor(0x07);
                    printf("MAINTENANCE");
                    break;
                }
                printf("\n");
            }
        }
        c = getch();
        if (c == -32) c = getch(); // Reduce flicker
        switch (c) {
        case 72:
            s--;
            break; // UP
        case 80:
            s++;
            break; // DOWN
        case 13: // ENTER
            if (car_status[s] != 1)
            {
                show_error("Unavailable at the moment. Please select another car");
                getch();
                break;
            }
            sel_car = s;
            return;
        }
        if (sel_location == 1) {
            if (s > 4) s = 4;
            if (s < 0) s = 0;
        } else if (sel_location == 2) {
            if (s > 9) s = 9;
            if (s < 5) s = 5;
        }
    }

}

void select_time() {

    info("Select the start time and end time.", "The program will calculate the duration for you,", "Note that any remaining minutes will be treated as one hour.", "");
    info("You will need to add a leading zero for hours below 10.", "4.30 am will be 04:30,", "When it asks for AM/PM, simply key in their initials", "(A or P) to choose it.");
    time_t t1, t2;
    int y=0, mo=0, d=0, h=0, m=0;
    int duration;
    int y2=0, mo2=0, d2=0, h2=0, m2=0;
    time_t now = current_time();

    while (1) {
        cls();

        create_banner("Select time", 0x1b, 1);
        gotoxy(9, 8);
        printf("Today is ");
        print_date(now);
        printf(", ");
        print_hour(now);
        gotoxy(8,11);
        setcolor(0x0e);
        printf("DATE START: ");
        setcolor(0x0f);
        printf("When do you want to book?\n\n");
        printf("     1. Today\n");
        printf("     2. Tomorrow\n");
        printf("     3. Select date\n");
        gotoxy(47, 11);
        int i;
        do
        {
            i = getd();
        } while (i < 1 && i > 3);

        if (i == 1)
        {
            t1 = now;
            gotoxy(20, 13);
            printf("--- on ");
            print_date(t1);
        }
        else if (i == 2)
        {
            t1 = now + 24 * 60 * 60; // Add 1 day
            gotoxy(20, 14);
            printf("--- on ");
            print_date(t1);
        }
        else if (i == 3)
        {
            int _;
            get_time(t1, &y, &_, &_, &_, &_);
            gotoxy(20, 15);
            printf("---> Please enter: dd / mm");
            gotoxy(39, 15);
            fflush(stdin);
            d = getd() * 10;
            d += getd();
            gotoxy(44, 15);
            fflush(stdin);
            mo = getd() * 10;
            mo += getd();

            if (mo > 12)
            {
                show_error("Invalid date (Month out of range)");
                continue;
            }

            if (d > max_month[mo])
            {
                show_error("Invalid date (Date out of range)");
                continue;
            }

            t1 = set_time(y, m, d, 0, 0);
        }

        gotoxy(15, 17);
        printf("ENTER TIME : hh:mm ");
        gotoxy(28, 17);
        fflush(stdin);
        h = getd() * 10;
        h += getd();
        gotoxy(31, 17);
        fflush(stdin);
        m = getd() * 10;
        m += getd();
        
        gotoxy(34, 17);
        printf("AM/PM?");
        char ampm;
        
        do
        {
        	ampm = getch();
        	if (ampm == 'a') ampm = 'A';
			if (ampm == 'p') ampm = 'P'; 
		} while (ampm != 'A' && ampm != 'P');
		
		if (ampm == 'A') 
		{
			gotoxy(34, 17);
			printf("AM     ");
		}
		else if (ampm == 'P') 
		{
			gotoxy(34, 17);
			printf("PM     ");
		}
		
        if (h > 12 || h < 1)
        {
            show_error("Invalid time");
            continue;
        }
        if (ampm == 'P')
        	h += 12;
        else if (ampm == 'A' && h == 12)
        	h = 0;
        	
        if (m > 60)
        {
            show_error("Invalid time");
            continue;
        }

        int unused;
        get_time(t1, &y, &mo, &d, &unused, &unused);
        t1 = set_time(y, mo, d, h, m);

        if (t1 < now)
        {
            show_error("Time entered had elapsed");
            continue;
        }

        t2 = t1;
        gotoxy(8, 20);
        setcolor(0x0e);
        printf("DATE END: ");
        setcolor(0x0f);
        printf("On how many days? (1 for same day, ... 9 to enter date) ");

        int days;
        do {
            days = getd();
        } while (days < 1 && days > 9);

        if (days != 9)
        {
            t2 += (days - 1) * 24 * 60 * 60;
            gotoxy(20, 21);
            printf("--- on ");
            print_date(t2);

        }
        else if (days == 9)
        {
            int _;
            get_time(t2, &y2, &_, &_, &_, &_);
            gotoxy(20, 21);
            printf("---> Please enter: dd / mm");
            gotoxy(39, 21);
            fflush(stdin);
            d2 = getd() * 10;
            d2 += getd();
            gotoxy(44, 21);
            fflush(stdin);
            mo2 = getd() * 10;
            mo2 += getd();
            get_time(t2, &y2, &_, &_, &_, &_);
            t2 = set_time(y2, mo2, d2, 0, 0);
            if (mo2 > 12)
            {
                show_error("Invalid date (Month out of range)");
                continue;
            }

            if (d2 > max_month[mo2])
            {
                show_error("Invalid date (Date out of range)");
                continue;
            }

        }
        gotoxy(15, 23);
        printf("ENTER TIME : hh:mm");
        gotoxy(28, 23);
        fflush(stdin);
        h2 = getd() * 10;
        h2 += getd();
        gotoxy(31, 23);
        fflush(stdin);
        m2 = getd() * 10;
        m2 += getd();

        gotoxy(34, 23);
        printf("AM/PM?");
        
        do
        {
        	ampm = getch();
        	if (ampm == 'a') ampm = 'A';
			if (ampm == 'p') ampm = 'P'; 
		} while (ampm != 'A' && ampm != 'P');
		
		if (ampm == 'A') 
		{
			gotoxy(34, 23);
			printf("AM     ");
		}
		else if (ampm == 'P') 
		{
			gotoxy(34, 23);
			printf("PM     ");
		}
		
        if (h2 > 12 || h2 < 1)
        {
            show_error("Invalid time");
            continue;
        }
        if (ampm == 'P')
        	h2 += 12;
        else if (ampm == 'A' && h2 == 12)
        	h2 = 0;

        if (m2 > 60)
        {
            show_error("Invalid time");
            continue;
        }

        get_time(t2, &y2, &mo2, &d2, &unused, &unused);
        t2 = set_time(y2, mo2, d2, h2, m2);

        if (t2 == t1) {
            show_error("Cannot enter same time and date!");
            continue;
        }

        if (t1 > t2) {
            show_error("Start time must be lower than end time!");
            continue;
        }
        if (t1 < now)
        {
            show_error("Time entered had elapsed");
            continue;
        }

        if (t2 - t1 > 7 * 24 * 60 * 60) {
            show_error("Cannot book for more than 1 week!");
            continue;
        }

        break;
    }
    duration = t2 - t1;
    duration = ceil((double) duration / 60 / 60);
    gotoxy(11, 25);
    printf("SERVICE DURATION: %d hours", duration);
    sel_time = t1;
    sel_duration = duration;
    getch();
}


/* User database module */
void load_user() {

    if (!fexists("users.txt"))
    {
        FILE *user = fopen("users.txt", "w");
        fprintf(user, "%d", 0);
        fclose(user);
    }
    else
    {
        FILE *user = fopen("users.txt", "r");
        fscanf(user, "%d\n", &num_user);
        int i;
        for (i = 0; i < num_user; i++)
        {
            fscanf(user, "%[^,], %[^,], %[^,], %d\n", &name[i], &phone[i], &ic[i], &usage[i]);
        }
    }

}

void add_user(char tn[50], char tp[50], char ti[50]) {
    strcpy(name[num_user], tn);
    strcpy(phone[num_user], tp);
    strcpy(ic[num_user], ti);
    usage[num_user] = 1;

    num_user++;
}

void save_user() {

    FILE *user = fopen("users.txt", "w");
    fprintf(user, "%d\n", num_user);
    int i;
    for (i = 0; i < num_user; i++)
    {
        fprintf(user, "%s, %s, %s, %d\n", name[i], phone[i], ic[i], usage[i]);
    }
    fclose(user);
}

void user_details() {

    info("Enter your user details.", "Your name and phone number will be used as an identification.", "You will get a promotion for each 5th booking you have taken", "");

    char tn[50], tp[50], ti[50];
start:
    while (1) {
        cls();
        create_banner("User details", 0x5f, 3);

        gotoxy(5, 9);
        printf("Username: ");
        fflush(stdin);
        gets(tn);

        if (strlen(tn) == 0)
        {
            show_error("Hey, type something");
            continue;
        } else if (strlen(tn) < 3) {
            show_error("Your name is too short");
            continue;
        }

        gotoxy(5, 11);
        printf("Phone number: ");
        fflush(stdin);
        gets(tp);
        if (!numbers_only(tp)) {
            show_error("Cannot enter letters/symbols for number field");
            continue;
        } else if (strlen(tp) == 0)
        {
            show_error("Hey, type something");
            continue;
        }
        else if (strlen(tp) < 6)
        {
            show_error("Phone number cannot be too short!");
            continue;
        }

        gotoxy(5, 13);
        printf("IC number: ");
        fflush(stdin);
        gets(ti);
        if (!numbers_only(ti)) {
            show_error("Cannot enter letters/symbols for number field");
            continue;
        }

        load_user();
        int i, status = 0;
        for (i = 0; i < num_user; i++)
        {
            if (strcmp(name[i], tn) == 0 && strcmp(phone[i], tp) == 0)
            {
                status = 1;
                break;
            } else if (strcmp(phone[i], tp) == 0)
            {
                status = 2;
                break;
            }
        }

        switch (status)
        {
        case 1:
            printf("%s %s", tn, tp);

            if (++usage[i] % 5 == 0)
            {
                get_promotions();
                getch();
            }
            break;
        case 2:
            show_error("Phone number already in use");
            continue;
        default:
            add_user(tn, tp, ti);
        }

        strcpy(sel_name, tn);
        strcpy(phoneno, tp);
        save_user();
        break;

    }

}

void get_promotions(){
    int i;
    for (i = 0; i < 8; i++)
    {
        cls();
        create_banner("You.ve got promotions!", 0x0f | (i * 16), 3);
        printf("\n\n\n\n    You have got 25%% off for this time!\n    How lucky!");
        Sleep(50);
    }
    cls();
    create_banner("You.ve got promotions!", 0x6f, 3);
    printf("\n\n\n\n    You have got 25%% off for this time!\n     How lucky!");

    getch();
    promotions = 1;
}


/* Payment module */

void price_table()
{
	info("Price rates for different time.", "There are packages for 6 hours, 12 hours... etc", "and they have a price reduction over normal ", "rates.");
	cls();
	create_banner("Price table", 0x0e, 5);
	printf("\n\n\n    Price rates for %s %s car", car_size[sel_car] == 'S' ? "small" : "large", car_automanual[sel_car] == 'A' ? "automatic" : "manual");
	gotoxy(8, 12);
	printf("  1 hour :   RM");
	gotoxy(8, 14);
	printf(" 6 hours :   RM");
	gotoxy(8, 16);
	printf("12 hours :   RM");
	gotoxy(8, 18);
	printf("   1 day :   RM");
	gotoxy(8, 20);
	printf("  1 week :   RM");
	
	if (car_size[sel_car] == 'S' && car_automanual[sel_car] == 'A')
	{
		gotoxy(25, 12);
		printf("%8.2f", rate1);
		gotoxy(25, 14);
		printf("%8.2f", rate1a);
		gotoxy(25, 16);
		printf("%8.2f", rate1b);
		gotoxy(25, 18);
		printf("%8.2f", rate1c);
		gotoxy(25, 20);
		printf("%8.2f", rate1d);
	}	
	else if (car_size[sel_car] == 'S' && car_automanual[sel_car] == 'M')
	{
		gotoxy(25, 12);
		printf("%8.2f", rate2);
		gotoxy(25, 14);
		printf("%8.2f", rate2a);
		gotoxy(25, 16);
		printf("%8.2f", rate2b);
		gotoxy(25, 18);
		printf("%8.2f", rate2c);
		gotoxy(25, 20);
		printf("%8.2f", rate2d);
	}
	else if (car_size[sel_car] == 'L' && car_automanual[sel_car] == 'A')
	{
		gotoxy(25, 12);
		printf("%8.2f", rate3);
		gotoxy(25, 14);
		printf("%8.2f", rate3a);
		gotoxy(25, 16);
		printf("%8.2f", rate3b);
		gotoxy(25, 18);
		printf("%8.2f", rate3c);
		gotoxy(25, 20);
		printf("%8.2f", rate3d);
	}
	else if (car_size[sel_car] == 'L' && car_automanual[sel_car] == 'M')
	{
		gotoxy(25, 12);
		printf("%8.2f", rate4);
		gotoxy(25, 14);
		printf("%8.2f", rate4a);
		gotoxy(25, 16);
		printf("%8.2f", rate4b);
		gotoxy(25, 18);
		printf("%8.2f", rate4c);
		gotoxy(25, 20);
		printf("%8.2f", rate4d);
	}
	getch();
}


void make_payment() {

    info("Payment details.", "They will be caculated base on the size and its transmission", "(auto/manual). Slight price cut for each increasing time", "");

    char transmission = car_automanual[sel_car];
    char size = car_size[sel_car];
    int duration = sel_duration;


    if (size == 'S' && transmission== 'A') {
        if (duration >= 168) {
            payment = (rate1d) + (duration - 168) * rate1;
        } else if (duration >= 24) {
            payment = (rate1c) + (duration - 24) * rate1;
        } else if (duration >= 12) {
            payment = (rate1b) + (duration - 12) * rate1;
        } else if (duration >= 6) {
            payment = (rate1a) + (duration - 6) * rate1;
        } else {
            payment = duration * rate1;
        }
    } else if (size == 'S' && transmission=='M') {
        if (duration >= 168) {
            payment = (rate2d) + (duration - 168) * rate2;
        } else if (duration >= 24) {
            payment = (rate2c) + (duration - 24) * rate2;
        } else if (duration >= 12) {
            payment = (rate2b) + (duration - 12) * rate2;
        } else if (duration >= 6) {
            payment = (rate2a) + (duration - 6) * rate2;
        } else {
            payment = duration * rate2;
        }
    } else if (size == 'L' && transmission=='A') {
        if (duration >= 168) {
            payment = (rate3d) + (duration - 168) * rate3;
        } else if (duration >= 24) {
            payment = (rate3c) + (duration - 24) * rate3;
        } else if (duration >= 12) {
            payment = (rate3b) + (duration - 12) * rate3;
        } else if (duration >= 6) {
            payment = (rate3a) + (duration - 6) * rate3;
        } else {
            payment = duration * rate3;
        }
    } else if (size == 'L' && transmission=='M') {
        if (duration >= 168) {
            payment = (rate4d) + (duration - 168) * rate4;
        } else if (duration >= 24) {
            payment = (rate4c) + (duration - 24) * rate4;
        } else if (duration >= 12) {
            payment = (rate4b) + (duration - 12) * rate4;
        } else if (duration >= 6) {
            payment = (rate4a) + (duration - 6) * rate4;
        } else {
            payment = duration * rate4;
        }
    }
    
    
    if (promotions == 1)
    	payment *= 0.75;

    int select = 1, selected = 1;
    char z;
    int i;

    char c;
    
    info ("Select a payment method.", "You may pay by cash and prepare the deposit to be paid later, or", "credit card. Note that cancelling the payment process will", "terminate this session");
    while (1) {
        cls();
        create_banner("Confirmation", 0x3f, 4);

        gotoxy(9, 8);
        printf("Location: %s", places[sel_location]);
        gotoxy(9, 9);
        printf("Car: %s [%s]", car_model[sel_car], car_plate[sel_car]);
        gotoxy(9, 10);
        printf("Time start: ");
        print_time(sel_time);
        gotoxy(9, 11);
        printf("Time end: ");
        print_time(sel_time + (sel_duration * 60 * 60));
        gotoxy(9, 12);
        printf("Duration: %d hour(s)", sel_duration);

        gotoxy(9, 14);
        printf("This is the amount you have to pay = %.2f\n", payment);
        //gotoxy(11,6);
        //printf("The car you have selected is %s\n",selected_car);
        gotoxy(11, 16);
        printf("Please select your payment options");
        gotoxy(11, 18);
        printf("%c * Cash", select == 1 ? '>' : ' ');
        gotoxy(11, 20);
        printf("%c * Credit card", select == 2 ? '>' : ' ');
        gotoxy(11, 22);
        printf("%c * CANCEL", select == 3 ? '>' : ' ');

        c = getch();
        switch (c) {
        case 72:
            select--;
            break; // UP
        case 80:
            select++;
            break; // DOWN
        case 13: // ENTER
            if (select == 1) {
                cls();
                create_banner("Payment", 0x3f, 5);
                gotoxy(14, 7);
                printf("How much you want to pay for deposit?\n");
                gotoxy(14, 8);
                scanf("%lf", & user_pay);
                total_payment = user_pay - payment;

				info("The balance shown here is the amount of remaining", "you have to pay later.", "However, the receipt will be generated for the deposit.", "");
				
                while (1) {
                    cls();
                    create_banner("Confirmation", 0x3f, 5);

                    gotoxy(14, 7);
                    printf("Your Balance = %.2f\n", total_payment);
                    gotoxy(11, 9);
                    printf("Are you sure you want to continue?\n");
                    gotoxy(11, 11);
                    printf("%c *YES\n", selected == 1 ? '>' : ' ');
                    gotoxy(11, 12);
                    printf("%c *NO\n", selected == 2 ? '>' : ' ');

                    z = getch();
                    switch (z) {
                    case 72:
                        selected--;
                        break; // UP
                    case 80:
                        selected++;
                        break; // DOWN
                    case 13: // ENTER
                        if (selected == 1) {
                            pay_via_creditcard = 0;
                            loading("Placing your booking...", 110);
                            print_receipt();
                        } else
                            printf("Exit.");
                        return;

                    default:
                        continue;
                    }

                    if (selected > 2) selected = 2;
                    if (selected < 1) selected = 1;
                }
            } else if (select == 2) {
            	info("Key in the credit card number.", "Randomized numbers are invalid. However for the", "sake of testing, you can enter the given number.", "They will be accepted too (for now).");

                cls();
                create_banner("Payment", 0x3f, 5);
                gotoxy(10, 8);
                printf("Enter a valid credit card number");
                gotoxy(10, 9);
                printf("Mastercard and Visa are supported");
                gotoxy(10, 11);
                printf("Example: 5555-5555-5555-4444");
				gotoxy(14, 15);
				printf("----------------");
				gotoxy(14, 15);
				
				char creditno[16];
				int i, k;
				
				for (i = 0; i < 16; i++)
                {
                	k = getd();
                	creditno[i] = k + '0'; // char to int
				}
				
				if (creditno[0] != '5' && creditno[0] != '4')
				{
					show_error("Only Mastercard or Visa supported");
					continue;
				} 
				if (verify_credit(creditno))
				{
					gotoxy(10, 18);
					printf("Validated!");
					gotoxy(10, 19);
					printf("Press ENTER to continue...");
					if (getch() != 13)
					{
						show_error("Cancelling...");
						continue;
					} 
					pay_via_creditcard = 1;
                    loading("Contacting card holder...", 600);
                    loading("Placing your booking...", 110);
                    print_receipt();
				}
				else
				{
					show_error("Invalid credit card number!");
					continue;
				}
            } else
            	exiting();
            return;
        }
        if (select > 3) select = 3;
        if (select < 1) select = 1;
    }

}

void print_receipt() {
    int i;
    loading("Generating receipt...", 20);
    FILE* fptr = fopen("receipt.txt", "w");
    srand(time(NULL));

    printf("Your receipt has been generated!");

    unsigned int r = rand() % 9000000 + 1000000;
    
    fprintf(fptr, "+-------------------------------------------------------------------+\n");
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|           C A R  R E N T A L  S D N  B H D                        |\n");
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|           Invoice no.%8u                ", r);
	fprint_time(fptr, current_time());
	fprintf(fptr, "       |\n");
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|      Recipient: %-30.30s                    |\n", sel_name);
    fprintf(fptr, "|     Contact no: %-30.30s                    |\n", phoneno);
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|     Car rented: [%-7.7s]  %-25.25s              |\n", car_plate[sel_car], car_model[sel_car]);
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|     Time for rent: ");
    fprint_time(fptr, sel_time);
    fprintf(fptr, "                                 |\n", r);
    fprintf(fptr, "|             until: ");
    fprint_time(fptr, sel_time + sel_duration * 60 * 60);
   	fprintf(fptr, "                                 |\n", r);
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|     Duration: %3d hour(s)                                         |\n", sel_duration);
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|      PAYMENT DETAILS                                              |\n");
    fprintf(fptr, "|      SUBTOTAL                              RM%12.2f         |\n", payment);

	if (pay_via_creditcard)
	{
	    fprintf(fptr, "|                                                                   |\n");
    	fprintf(fptr, "|      Paid by credit card (Ref No #%08d)                       |\n", rand() % 100000000);
		fprintf(fptr, "|                                                                   |\n");
	} else
	{
	    fprintf(fptr, "|      DEPOSIT                               RM%12.2f         |\n", user_pay);
	    fprintf(fptr, "|    --------------------------------------------------------       |\n");
	    fprintf(fptr, "|      TOTAL                                 RM%12.2f         |\n", total_payment);
	}
    fprintf(fptr, "|    --------------------------------------------------------       |\n");
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "|                                                                   |\n");
    fprintf(fptr, "+-------------------------------------------------------------------+\n");
    
    fclose(fptr);
    show_info("Done printing your receipt.", "You can check it now by opening", "\"receipt.txt\" file.", "Please store them for your reference.");
    show_info("BOOKING SUCCESSFUL", "You can retrieve your car at the scheduled time.", "Please bring your IC and driver licence copy too", "Thank you and have a good day :)");
}


void exiting() {
    info("Thanks for using this program.", "The program will now closing, and this infobox will not be", "shown again. You may turn this on again by pressing", "[H] on the title screen next time");
    cls();
    exit(0);
}


// END OF FILE car_rental.c
