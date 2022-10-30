/*  Program:    calendar.c
 *  Author:     Anton Sixtenson
 *  Brief:      Somewhat of a cal clone
 *  Tested on:  Linux
 *  
 *  How to use:
 *   [compiled program] [options]
 *
 *   Running program without arguments will print current month
 *
 *   Options:
 *     -y <num>       Year to print
 *                      Note: Prints whole year if -m is not specified
 *     -m <num>       Month to print
 *                      Note: January = 0 
 *     -w             Print week numbers
 *     -n <num>       Number of months to print
 *                      Note: Will only print until end of year
 *                            Starts from current month if -m is not specified
 *                            Prints whole year if used with -y without -m
 *     -h             Display this help page
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char *month_name[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char *day_names = "Su Mo Tu We Th Fr Sa";
int num_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


 /* COLOR CODES
  *
  * Colors for current day
  */
#define WHTB "\033[30m\033[47m"

/*
 * Reset output colors
 */
#define RST "\033[0m"


/* FUNCTION:    get_current_date
 * Brief:       Get current date 
 * Return:
 *          Pointer to int array:
 *           date[0] = day
 *           date[1] = month
 *           date[2] = year
 */
int *get_current_date(){
    static int date[3];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date[0] = tm.tm_mday;
    date[1] = tm.tm_mon;
    date[2] = tm.tm_year+1900;
    return date;
}


/*  FUNCTION:   is_leap_year
 *  Brief:      Calculate if a year is leap year
 *  Param:  
 *              y: year
 *
 *  Return:     1 if provided year is a leap year, 0 if it is not.
 */
int is_leap_year(int y){
    return ((y%4 == 0 && y%100 != 0) || y%400 == 0);
}

/*  FUNCTION:   month_start_day
 *  Brief:      Calculate what day a month starts with at a specific year.
 *              Assumes 01.01.01 was a Monday.
 *  Param:  
 *              y: year
 *              m: month (where 0=January, 1=February...)
 *
 *  Return:     Day of week, where 0=Sunday, 1=Monday ... 6=Saturday
 */
int month_start_day(int y, int m){
    /* 01.01.01 is a Monday */
    int total_days = 1;

    /* Calculate days passed until year y*/
    total_days += (y-1)*365;
    int leaps = ((y-1)/4) + ((y-1)/400) - ((y-1)/100);
    total_days += leaps;

    /* Calculate days passed until month m of year y */
    for(int i = 0; i < m; i++){
        total_days += num_days[i];
    }

    return total_days%7;
}


/*  FUNCTION:   month_start_week
 *  Brief:      Calculate what week a month at a specific year starts with
 *  Param:
 *              y: year
 *              m: month
 *  
 *  Return:     Week number the month starts with.
 */
int month_start_week(int y, int m){
    int total_days = month_start_day(y, 0);
    int week = 1;
    for(int i = 0; i < m; i++){
        total_days += num_days[i];
    }
    
    week += total_days/7;
    return week;   
}


/*  FUNCTION:   year_char_len
 *  Brief:      Calculate number of characters in a integer
 *  Param: 
 *              y: year (any integer) 
 *         
 *  Return:     Number of characters the integer consists of. (2022 would return 4).
 */
int year_char_len(int y){
    int count = 0;
    while(y != 0){
        y /= 10;
        count++;
    }

    return count;
}

/*  FUNCTION:   print_spaces
 *  Brief:      Print n number of spaces
 *  Param:
 *              n: number of spaces
*/
void print_spaces(int n){
    for(int i = 0; i < n; i++){
        printf(" ");
    }
}

/*  FUNCTION:   print_heading
 *  Brief:      Print formatted heading for calendar
 *  Param:
 *              y: year
 *              m: month (to start print at)
 *              n: number of headings to print
 *              w: If set to 1 - format heading to fit calendar with week numbers. 
                s: If set to 1 - print both month and year in heading
 */
void print_heading(int y, int m, int n, int w, int s){
    int include_year = 0;
    /* If year is to be included in heading */
    if(s == 1){
        include_year = year_char_len(y) + 1;
    }
    /* Calculate spaces and print month names */
    for(int i = 0; i < n; i++){
        int month_name_len = strlen(month_name[m+i]);
        int num_spaces  = (20-(month_name_len+include_year))/2;
        int remainder = (20-(month_name_len+include_year))%2;
        /* If w is set to 1 (i.e print calendar with week numbers)
           Print three additional spaces to get correct formatted output 
        */
        print_spaces(num_spaces+(w*3));
        printf("%s", month_name[m+i]);
        if(s == 1){
            printf(" %d", y);
        }
        /* Add extra space if week numbers are to be included in output (+w)*/
        print_spaces(num_spaces+remainder+2+w);
    }

    printf("\n");
    /* Print the name of days (Su Mo...)
        And if w is set format to fit the inclusion of week numbers
    */
    for(int j = 0; j < n; j++){
        if(w)
            print_spaces(3);
        printf("%s", day_names);
        print_spaces(2+w);
    }

    printf("\n");
}


/*  FUNCTION:   print_day_numbers
 *  Brief:      Prints three months in a unix cal formatted way
 *  Param:
 *              y: year
 *              m: month (to start print at)
 *              n: number of months to print
 *              w: If set to 1 - include week numbers. 
 *           
 */
void print_day_numbers(int y, int m, int n, int w){

    int remaining_days = 0;
    int start_day[n];
    int days_printed[n];
    int week[n];
    int *date = get_current_date();

    /* Set variable values */
    for(int i = 0; i < n; i++){
        remaining_days += num_days[m+i];
        start_day[i] = month_start_day(y, m+i);
        days_printed[i] = 1;
        if(w)
            week[i] = month_start_week(y, m+i);
    }

    /* Decrement n to prevent segmentation fault */
    n--;

    int month_pointer = 0;
    int day_pointer = 0;

    /* Print day (and week) numbers loop */
    while(remaining_days > 0){
        /* Print week number */
        if(w){
            if(day_pointer == 0 && days_printed[month_pointer] <= num_days[month_pointer+m]){
                if(week[month_pointer] < 10){
                    print_spaces(1);
                }
                printf("%d ", week[month_pointer]);
                week[month_pointer]++;
            } else if(day_pointer == 0){
                    print_spaces(3);
            }
        }
        /* Handle cases where there should not be any number printed */
        if(start_day[month_pointer] > 0){
            print_spaces(start_day[month_pointer]*3);
            day_pointer = start_day[month_pointer];
            start_day[month_pointer] = -1;
        } else if(days_printed[month_pointer] > num_days[month_pointer+m]){
            print_spaces((7-day_pointer)*3);
            day_pointer = 7;
        } else {
            /* Set color (and print) if date to be printed is the current date */
            if(y == date[2] && m+month_pointer == date[1] && days_printed[month_pointer] == date[0]){
                printf(WHTB);
                if(days_printed[month_pointer] < 10){
                    print_spaces(1);
                }
                printf("%d", days_printed[month_pointer]);
                printf(RST);
                print_spaces(1);
                days_printed[month_pointer]++;
                day_pointer++;
                remaining_days--;
            /* Print all other day numbers (not current date)*/
            } else {
                if(days_printed[month_pointer] < 10){
                    print_spaces(1);
                }
                printf("%d ", days_printed[month_pointer]);
                days_printed[month_pointer]++;
                day_pointer++;
                remaining_days--;
            }
        }
        /* Move to next month */
        if(day_pointer%7 == 0 && month_pointer != n){
            print_spaces(1+w);
            month_pointer++;
            day_pointer = 0;
        /* Move to new line */
        } else if(day_pointer%7 == 0 && month_pointer == n){
            printf("\n");
            month_pointer = 0;
            day_pointer = 0;
        }
    }
    printf("\n");
}

/*  FUNCTION:   print_calendar
 *  Brief:      Uses three functions to print n (up to 12) number of months
 */
void print_calendar(int y, int m, int n, int w, int s){
    num_days[1] = (is_leap_year(y)) ? 29 : 28;
    print_heading(y, m, n, w, s);
    print_day_numbers(y, m, n, w);
}

/*  FUNCTION:   print_help
 *  Brief:      Print formatted how-to-use instructions
 */
void print_help(){
    printf("How to use:\n[compiled program] [options]\n\nRunning program without arguments will print current month\n\nOptions:\n -y <num>\tYear to print\n\t\t  Note: Prints whole year if -m is not specified\n -m <num>\tMonth to print\n\t\t  Note: January = 0\n -w\t\tPrint week numbers\n -n <num>\tNumber of months to print\n\t\t  Note: Will only print until end of year\n\t\t\tStarts from current month if -m is not specified\n\t\t\tPrints whole year if used with -y without -m\n -h\t\tDisplay this help page\n");
}

/*  FUNCTION:   print_year
 *  Brief:      Print the whole year with year heading
 *  Param:      
 *              y: year
 *              w: If set (to 1) include week numbers
 */
void print_year(int y, int w){
    int heading_len = (w) ? 78 : 64;
    int year_len = year_char_len(y);
    int num_spaces = (heading_len-year_len)/2;
    printf("\n");
    print_spaces(num_spaces);
    printf("%d", y);
    printf("\n\n");
    for(int i = 0; i < 4; i++){
        print_calendar(y, i*3, 3, w, 0);
        printf("\n");
    }
}

/*  FUNCTION:   run
 *  Brief:      Handle input arguments and run program accordingly
 *  Param:      
 *              y: year
 *              m: month (to start print from)
 *              n: How many month (forward) to print. Or until end of year.
 *                 Ex. -n 13 prints (with no -m specified) prints remaining month of current year. 
 *                 Ex 2. -m 10 -n 3 will only print last two months.
 *              w: If set (to 1) include week numbers
 */
int run(int y, int m, int n, int w){
    int *date = get_current_date();
    int s = 0;
    if(y > 0 && m < 0){
        print_year(y, w);
        return 0;
    }
    if(y < 1){
        y = date[2];
    }
    if(m < 0){
        m = date[1];
    }
    if(n == 12){
        print_year(y, w);
        return 0;
    } else if(m+n > 12){
        n = 12 - m;
    } else if(n < 1){
        n = 1;
    }
    if(n > 3){
        while(n > 3){
            printf("\n");
            print_calendar(y, m, 3, w, 0);
            m += 3;
            n -= 3;
        }
        printf("\n");
        print_calendar(y, m, n, w, 0);
        return 0;
    }
    if(n == 1){
        s = 1;
    }
    print_calendar(y, m, n, w, s);
    return 0;
}


int main(int argc, char *argv[]){
    int y = 0, m = -1, n = 0, w = 0;

    for(int i = 1; i < argc; i++){
        char c = argv[i][1];
        switch(c){
            case 'w':
                w = 1;
                break;
            case 'n':
                if(argv[i+1]){
                    n = atoi(argv[i+1]);
                    i += 1;
                    break;
                } else {
                    print_help();
                    return 0;
                }
            case 'm':
                if(argv[i+1]){
                    m = atoi(argv[i+1]);
                    i += 1;
                    break;
                } else {
                    print_help();
                    return 0;
                }
            case 'y':
                if(argv[i+1]){
                    y = atoi(argv[i+1]);
                    i += 1;
                    break;
                } else {
                    print_help();
                    return 0;
                }
            default:
                print_help();
                return 0;
        }
    }

    run(y, m, n, w);
    
    return 0;
}