# Calendar
Somewhat of a cal clone


## How to use

[compiled program] [options]

Running program without arguments will print current month

Options:
    -y <num>       Year to print
                      Note: Prints whole year if -m is not specified
    -m <num>       Month to print
                      Note: January = 0 
    -w             Print week numbers
    -n <num>       Number of months to print
                      Note: Will only print until end of year
                            Starts from current month if -m is not specified
                            Prints whole year if used with -y without -m
    -h             Display this help page
