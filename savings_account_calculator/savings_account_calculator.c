#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Function declarations */
void menu();
void continueOrExit();
double getValidInput(const char* prompt);
int getValidIntInput(const char* prompt);

/* Main function */
int main()
{
    menu();
    return 0;
}

/* Function to validate double input */
double getValidInput(const char* prompt) {
    double value;
    printf("%s", prompt);
    while (scanf("%lf", &value) != 1 || value <= 0) {
        printf("Invalid input. Please enter a valid positive number.\n");
        while (getchar() != '\n');
    }
    return value;
}

/* Function to validate integer input */
int getValidIntInput(const char* prompt) {
    int value;
    char ch1;

    printf("%s", prompt);
    while (1) {
        if (scanf("%d", &value) != 1 || value <= 0) {
            printf("Invalid input. Please enter a valid positive integer.\n");
            while (getchar() != '\n');
        } else {
            if (scanf("%c", &ch1) == 1 && ch1 != '\n') {
                printf("Invalid input. Please enter a valid positive integer.\n");
                while (getchar() != '\n');
            } else {
                break;
            }
        }
    }
    return value;
}

/* Function to ask if the user wants to perform another calculation */
void continueOrExit() {
    char choice1;
    while (1) {
        printf("\nWould you like to perform another calculation? (y/n): ");
        if (scanf(" %c", &choice1) == 1) {
            if (choice1 == 'y' || choice1 == 'Y') {
                printf("\nReturning to menu...\n");
                menu();
                break;
            } else if (choice1 == 'n' || choice1 == 'N') {
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            } else {
                printf("Invalid input. Please enter 'y' for yes or 'n' for no.\n");
            }
        }
    }
}

/* Main menu function */
void menu() {
    int choice;
    char ch;
    printf("1. Enter P, r, n, t. Find B.\n");
    printf("2. Enter r, n, t, B. Find P.\n");
    printf("3. Enter n, t, P, B. Find r.\n");
    printf("4. Enter t, r, P, B. Find n.\n");
    printf("5. Enter n, r, P, B. Find t.\n");
    printf("6. Generate report for given year interval.\n");
    printf("7. Compare two accounts.\n");
    printf("0. Exit program.\n");

    while (1) {
        printf("Enter your option (0-7): ");
        if (scanf("%d%c", &choice, &ch) == 2 && ch == '\n') {
            if (choice >= 0 && choice <= 7) {
                if (choice == 0) {
                    printf("Exiting the program. Goodbye!\n");
                    exit(0);
                }
                break;
            } else {
                printf("Invalid input. Please enter an integer between 0 and 7.\n");
            }
        } else {
            printf("Invalid input. Please enter an integer between 0 and 7, without any decimals.\n");
            while (getchar() != '\n');
        }
    }
    
    if (choice == 1) {
        double P, r, B, t;
        int n;
        printf("Option 1 has been selected: Find B.\n");

        P = getValidInput("Enter the principal invested (P): ");
        r = getValidInput("Enter the interest rate (r in decimal): ");
        n = getValidIntInput("Enter the compounding frequency per year (n): ");
        t = getValidInput("Enter the number of years of investment (t): ");

        B = P * pow((1 + r / n), n * t);
        printf("The balance (B) is: %.2f\n", B);
        continueOrExit();

    } else if (choice == 2) {
        double P, r, B, t;
        int n;
        printf("Option 2 has been selected: Find P.\n");

        r = getValidInput("Enter the interest rate (r in decimal): ");
        n = getValidIntInput("Enter the compounding frequency per year (n): ");
        t = getValidInput("Enter the number of years of investment (t): ");
        B = getValidInput("Enter the balance (B): ");

        P = (B / pow((1 + r / n), n * t));
        printf("The Principal invested (P) is: %.2f\n", P);
        continueOrExit();

    } else if (choice == 3) {
        double P, r, B, t;
        int n;
        printf("Option 3 has been selected: Find r.\n");

        n = getValidIntInput("Enter the compounding frequency per year (n): ");
        t = getValidInput("Enter the number of years of investment (t): ");
        P = getValidInput("Enter the principal invested (P): ");
        B = getValidInput("Enter the balance (B): ");

        r = n * (exp(log(B / P) / (n * t)) - 1);
        printf("The interest rate (r in decimal) is: %.3f\n", r);
        continueOrExit();

    } else if (choice == 4) {
        double P, r, B, t;
        int n = -1, i;
        printf("Option 4 has been selected: Find n.\n");

        t = getValidInput("Enter the number of years of investment (t): ");
        r = getValidInput("Enter the interest rate (r in decimal): ");
        P = getValidInput("Enter the principal invested (P): ");
        B = getValidInput("Enter the balance (B): ");

        for (i = 1; i <= 12; i++) {
            double j = P * pow((1 + r / i), (i * t));
            if (fabs(j - B) < 0.01) {
                n = i;
                break;
            }
        }

        if (n != -1) {
            printf("The compounding frequency per year (n) is: %d\n", n);
        } else {
            printf("No compounding frequency found that meets the balance criteria.\n");
        }
        continueOrExit();

    } else if (choice == 5) {
        double P, r, B, t;
        int n;
        printf("Option 5 has been selected: Find t.\n");

        n = getValidIntInput("Enter the compounding frequency per year (n): ");
        r = getValidInput("Enter the interest rate (r in decimal): ");
        P = getValidInput("Enter the principal invested (P): ");
        B = getValidInput("Enter the balance (B): ");

        t = log(B / P) / (n * log(1 + r / n));
        printf("The time interval (t) is approximately: %.2f years.\n", t);
        continueOrExit();

    } else if (choice == 6) {
        double P, r, B;
        int n, t1, t2;
        printf("Option 6 has been selected: Generate report for given year interval.\n");

        P = getValidInput("Enter the principal invested (P): ");
        r = getValidInput("Enter the interest rate (r in decimal): ");
        n = getValidIntInput("Enter the compounding frequency per year (n): ");
        t1 = getValidIntInput("Enter the start year (t1): ");
        t2 = getValidIntInput("Enter the end year (t2): ");

        if (t2 < t1) {
            printf("Invalid input for end year. Please ensure that the end year is greater than or equal to the start year.\n");
            printf("Enter the end year (t2): ");
            scanf("%d", &t2);
        }
        printf("%-6s %-15s %-15s %-20s %-12s\n", "Year", "Principal", "Interest rate", "Compound ratio", "Balance");

        for (int t = t1; t <= t2; t++) {
            B = P * pow((1 + r / n), n * t);
            printf("%-6d %-15.2f %-15.3f %-20d %-12.2f\n", t, P, r, n, B);
        }

        continueOrExit();

    } else if (choice == 7) {
        double P, r, balance1, balance2;
        int n1, n2, t1, t2_years = 0, t2_months = 0;
        double t2_total_months = 0.0;
        double smallest_diff = -1;
        int best_t2_years = 0, best_t2_months = 0;
        printf("Option 7 has been selected: Compare two accounts.\n");

        P = getValidInput("Enter the principal invested (P): ");
        r = getValidInput("Enter the interest rate (r in percent): ");
        t1 = getValidIntInput("Enter the number of years of investment for the first account (t1): ");
        n1 = getValidIntInput("Enter the number of times interest is compounded per year for the first account (n1): ");
        n2 = getValidIntInput("Enter the number of times interest is compounded per year for the second account (n2): ");

        balance1 = P * pow((1 + r / (100 * n1)), n1 * t1);
        printf("Balance in the first account after %d years: %.2f\n", t1, balance1);

        balance2 = P;

        while (1) {
            balance2 = P * pow((1 + r / (100 * n2)), n2 * t2_total_months / 12.0);
            double diff = fabs(balance1 - balance2);

            if (smallest_diff == -1 || diff < smallest_diff) {
                smallest_diff = diff;
                best_t2_years = t2_years;
                best_t2_months = t2_months;
            }

            if (balance2 >= balance1) {
                break;
            }

            t2_months++;
            t2_total_months++;

            if (t2_months == 12) {
                t2_months = 0;
                t2_years++;
            }
        }
        printf("It would take approximately %d years and %d months for the balance in the second account to get as close as possible to the balance in the first account.\n", best_t2_years, best_t2_months);
        continueOrExit();
    }
}
