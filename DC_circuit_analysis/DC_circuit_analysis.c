#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <math.h>

/* -------------------------- */
/*        Structure Definitions       */
/* -------------------------- */

/* Maximum number of resistors allowed */
#define MAX_RESISTORS 5

/* VoltageSource structure stores details of a voltage source in the circuit */
typedef struct {
    int positive_node;      // Positive terminal node of the voltage source
    int negative_node;      // Negative terminal node of the voltage source
    double value;           // Value of the voltage source (in volts)
    char type[10];          // Type of voltage source (e.g., DC, AC)
} VoltageSource;

/* Resistor structure stores details of a resistor in the circuit */
typedef struct {
    int positive_node;      // Positive terminal node of the resistor
    int negative_node;      // Negative terminal node of the resistor
    double value;           // Value of the resistor (in ohms)
} Resistor;

/* -------------------------- */
/*     Global Variables       */
/* -------------------------- */

VoltageSource voltageSource;                // Voltage source object
Resistor resistors[MAX_RESISTORS];            // Array of resistors
int resistorCount = 0;                        // Number of resistors loaded/created
int circuitLoaded = 0;                        // Flag to track if a circuit has been loaded
int circuitCreatedOrLoaded = 0;               // Flag to track if circuit has been created or loaded

/* -------------------------- */
/*     Function Prototypes    */
/* -------------------------- */

int isNumericInput(const char *input);
double getValidDoubleInput(const char *prompt);
int getPositiveWholeNumber(const char *prompt);
int getValidResistorCount(const char *prompt);
void createCircuit();
void saveCircuit();
void listSavedCircuits();
void loadCircuit();
void analyzeAndPrintReport();
void displayMenu();

/* -------------------------- */
/*          Main Function     */
/* -------------------------- */

int main() {
    int choice;              // Variable to store user choice
    char buffer[100];        // Buffer to store input temporarily

    do {
        displayMenu();       // Display the menu to the user

        while (1) {
            if (scanf("%s", buffer) != 1) {  // Read input as a string
                printf("\nInput error. Please try again.\n");
                while (getchar() != '\n');   // Clear invalid input from the buffer
                displayMenu();               // Show the menu again
                continue;                    // Prompt the user again
            }

            // Check if the input contains a decimal point (invalid for this input)
            if (strchr(buffer, '.') != NULL) {
                printf("\nInvalid input. Please enter a whole number without decimal places.\n");
                displayMenu();               // Show the menu again
                continue;                    // Prompt the user again
            }

            // Attempt to parse the input as an integer and check if it's within valid range
            if (sscanf(buffer, "%d", &choice) != 1 || choice < 1 || choice > 5) {
                printf("\nInvalid input. Please enter a whole number between 1 and 5.\n");
                displayMenu();               // Show the menu again
                continue;                    // Prompt the user again
            }

            // Valid input received, break out of the loop
            break;
        }

        // Execute action based on user choice
        switch (choice) {
            case 1:
                createCircuit();         // Option to create a circuit
                /* Reset circuitLoaded flag since a new circuit is created */
                circuitLoaded = 0;
                break;
            case 2:
                loadCircuit();           // Option to load a saved circuit
                break;
            case 3:
                saveCircuit();           // Option to save the current circuit
                break;
            case 4:
                analyzeAndPrintReport(); // Option to analyze and print the report
                break;
            case 5:
                printf("\nExiting program. Goodbye!\n");  // Exit the program
                break;
            default:
                printf("\nInvalid choice. Please select an option between 1 and 5.\n");
        }
    } while (choice != 5);  // Repeat the loop until the user selects option 5 to exit

    return 0;
}

/* -------------------------- */
/*       Utility Functions    */
/* -------------------------- */

/* Checks if the input is a valid numeric value */
int isNumericInput(const char *input) {
    int hasDecimal = 0;  // Flag to track if decimal point is encountered
    for (size_t i = 0; i < strlen(input); i++) {
        if (input[i] == '.') {
            if (hasDecimal) {
                return 0;  // More than one decimal point is invalid
            }
            hasDecimal = 1;  // Mark that a decimal point is found
        } else if (!isdigit(input[i])) {
            return 0;  // If the character is not a digit or decimal, it's invalid
        }
    }
    return 1;  // Valid number
}

/* Prompt the user for a valid positive double input */
double getValidDoubleInput(const char *prompt) {
    char input[100];  // String to store user input
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%99s", input) == 1) {
            if (isNumericInput(input)) {
                value = atof(input);
                if (value > 0) {
                    return value;
                }
            }
        }
        printf("Error: Please enter a valid positive number.\n");
        while (getchar() != '\n');  // Clear input buffer
    }
}

/* Prompt the user for a positive whole number */
int getPositiveWholeNumber(const char *prompt) {
    char input[100];  // String to store user input
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%99s", input) == 1) {
            if (isNumericInput(input)) {
                value = atoi(input);
                if (value > 0) {
                    return value;
                }
            }
        }
        printf("Error: Please enter a positive whole number.\n");
        while (getchar() != '\n');  // Clear input buffer
    }
}

/* Get a valid resistor count between 3 and 5 */
int getValidResistorCount(const char *prompt) {
    char input[100];  // String to store user input
    int count;

    while (1) {
        printf("%s", prompt);
        scanf("%s", input);

        if (isNumericInput(input)) {  
            count = atoi(input);  
            if (count >= 3 && count <= 5) {  
                return count;
            }
        }
        printf("Error: Number of resistors must be between 3 and 5 and a valid positive whole number.\n");
        while (getchar() != '\n');  // Clear input buffer
    }
}

/* -------------------------- */
/*       Circuit Functions    */
/* -------------------------- */

/* Create a new circuit (either SERIES or PARALLEL) */
void createCircuit() {
    char circuitType[10];  // String to store the type of the circuit

    // Ask the user for the type of circuit
    printf("Enter the type of circuit (SERIES or PARALLEL): ");
    scanf("%s", circuitType);

    // Convert the input to uppercase for case-insensitivity
    for (int i = 0; circuitType[i]; i++) {
        circuitType[i] = toupper(circuitType[i]);
    }

    // Check if the input is valid (SERIES or PARALLEL)
    if (strcmp(circuitType, "SERIES") != 0 && strcmp(circuitType, "PARALLEL") != 0) {
        printf("Invalid circuit type. Please enter either SERIES or PARALLEL.\n");
        return;
    }

    // Set voltage source connections based on circuit type
    if (strcmp(circuitType, "SERIES") == 0) {
        voltageSource.positive_node = 1;
        voltageSource.negative_node = resistorCount + 2;
    } else if (strcmp(circuitType, "PARALLEL") == 0) {
        voltageSource.positive_node = 1;
        voltageSource.negative_node = 2;
    }

    // Get the voltage value from the user
    voltageSource.value = getValidDoubleInput("Enter voltage value (in volts): ");

    // Get the number of resistors for the circuit
    resistorCount = getValidResistorCount("Enter number of resistors (between 3 and 5): ");

    // Loop through each resistor and get its resistance value
    for (int i = 0; i < resistorCount; i++) {
        printf("\nEnter resistance value for resistor R%d (in ohms): ", i + 1);
        resistors[i].value = getValidDoubleInput("Resistance value: ");

        // Set nodes for the resistors based on circuit type and resistor count
        if (strcmp(circuitType, "SERIES") == 0) {
            if (resistorCount == 3) {
                resistors[i].positive_node = i + 1;
                resistors[i].negative_node = i + 2;
            } else if (resistorCount == 4) {
                if (i == 0) {
                    resistors[i].positive_node = 1;
                    resistors[i].negative_node = 2;
                } else if (i == 1) {
                    resistors[i].positive_node = 1;
                    resistors[i].negative_node = 2;
                } else if (i == 2) {
                    resistors[i].positive_node = 2;
                    resistors[i].negative_node = 3;
                } else {
                    resistors[i].positive_node = 3;
                    resistors[i].negative_node = 4;
                }
            } else if (resistorCount == 5) {
                if (i == 0) {
                    resistors[i].positive_node = 1;
                    resistors[i].negative_node = 2;
                } else if (i == 1) {
                    resistors[i].positive_node = 1;
                    resistors[i].negative_node = 2;
                } else if (i == 2) {
                    resistors[i].positive_node = 2;
                    resistors[i].negative_node = 3;
                } else if (i == 3) {
                    resistors[i].positive_node = 2;
                    resistors[i].negative_node = 3;
                } else {
                    resistors[i].positive_node = 3;
                    resistors[i].negative_node = 4;
                }
            }
        } else if (strcmp(circuitType, "PARALLEL") == 0) {
            // For parallel circuit, nodes are the same for each resistor
            resistors[i].positive_node = 1;
            resistors[i].negative_node = 2;
        }
    }

    // Copy the circuit type into the voltage source type field
    strncpy(voltageSource.type, circuitType, sizeof(voltageSource.type));

    // Print the created circuit details
    printf("\nCircuit created successfully.\n");
    printf("Circuit Type: %s\n", circuitType);
    printf("Voltage Source: %d -> %d, Voltage: %.2f Volts\n",
           voltageSource.positive_node, voltageSource.negative_node, voltageSource.value);

    // Print resistor details
    for (int i = 0; i < resistorCount; i++) {
        printf("Resistor R%d: %d -> %d, Resistance: %.2f Ohms\n",
               i + 1, resistors[i].positive_node, resistors[i].negative_node, resistors[i].value);
    }

    /* Mark that a circuit has been created */
    circuitCreatedOrLoaded = 1;
}

/* Save the current circuit configuration to a file */
void saveCircuit() {
    if (resistorCount <= 0 || voltageSource.value <= 0) {
        printf("Error: No circuit has been created yet. Please create a circuit first (Option 1).\n");
        return;
    }

    FILE *file;
    char filename[100];

    /* Prompt user for a valid filename with a .cir extension */
    while (1) {
        printf("Enter filename to save the circuit (e.g., circuit.cir): ");
        scanf("%s", filename);

        char *extension = strrchr(filename, '.');
        if (extension && strcmp(extension, ".cir") == 0) {
            break;
        }
        printf("Error: The file must have a '.cir' extension. Please try again.\n");
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }

    /* Save the circuit type and voltage source details */
    fprintf(file, "%s\n", voltageSource.type);
    fprintf(file, "Voltage Source: %d -> %d, Type: DC, Voltage: %.2f\n",
            voltageSource.positive_node, voltageSource.negative_node, voltageSource.value);

    /* Save each resistor's details */
    for (int i = 0; i < resistorCount; i++) {
        fprintf(file, "Resistor %d: %d -> %d, Resistance: %.2f\n",
                i + 1, resistors[i].positive_node, resistors[i].negative_node, resistors[i].value);
    }

    printf("Circuit saved to %s successfully.\n", filename);
    fclose(file);
}

/* List all saved circuit files (.cir) in the current directory */
void listSavedCircuits() {
    struct dirent *entry;
    DIR *dp = opendir(".");
    if (dp == NULL) {
        printf("Error: Unable to open the directory.\n");
        return;
    }

    printf("\nAvailable Saved Circuits:\n");
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".cir") != NULL) {
            printf("  - %s\n", entry->d_name);
        }
    }
    closedir(dp);
}

/* Load a saved circuit from a file */
void loadCircuit() {
    FILE *file;
    char filename[100];
    int fileLoaded = 0;

    while (!fileLoaded) {
        listSavedCircuits();   // List available saved circuits

        printf("\nEnter filename to load the circuit (e.g., circuit.cir): ");
        scanf("%s", filename);

        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: File '%s' not found. Please choose a file from the list.\n", filename);
            continue;
        }

        resistorCount = 0;   // Reset resistor count

        char circuitType[20];
        if (fgets(circuitType, sizeof(circuitType), file) == NULL) {
            printf("Error reading circuit type from file. Please choose another file.\n");
            fclose(file);
            continue;
        }
        circuitType[strcspn(circuitType, "\n")] = 0;
        printf("Circuit Type: %s\n", circuitType);

        if (fscanf(file, "Voltage Source: %d -> %d, Type: DC, Voltage: %lf\n",
                   &voltageSource.positive_node, &voltageSource.negative_node,
                   &voltageSource.value) != 3) {
            printf("Error reading voltage source from file. Please choose another file.\n");
            fclose(file);
            continue;
        }

        while (fscanf(file, "Resistor %*d: %d -> %d, Resistance: %lf\n",
                      &resistors[resistorCount].positive_node,
                      &resistors[resistorCount].negative_node,
                      &resistors[resistorCount].value) == 3) {
            resistorCount++;
            if (resistorCount >= MAX_RESISTORS) {
                printf("Warning: Maximum resistor count exceeded. Some resistors may not be loaded.\n");
                break;
            }
        }

        /* Display the loaded circuit details */
        printf("\nLoaded Circuit Details:\n");
        printf("Voltage Source: %d -> %d, Type: DC, Voltage: %.2f Volts\n",
               voltageSource.positive_node, voltageSource.negative_node, voltageSource.value);

        for (int i = 0; i < resistorCount; i++) {
            printf("Resistor %d: %d -> %d, Resistance: %.2f Ohms\n",
                   i + 1, resistors[i].positive_node, resistors[i].negative_node, resistors[i].value);
        }
        circuitLoaded = 1;
        circuitCreatedOrLoaded = 1;

        fclose(file);
        fileLoaded = 1;
    }
}

/* Analyze the circuit and print a report for DC analysis */
void analyzeAndPrintReport() {
    if (!circuitCreatedOrLoaded) {
        printf("Error: No circuit has been created or loaded. Please use Option 1 (Create Circuit) or Option 2 (Load Circuit) first.\n");
        return;
    }

    double totalResistance = 0.0;
    double totalCurrent = 0.0;
    double currents[MAX_RESISTORS] = {0};
    double voltageDrops[MAX_RESISTORS] = {0};
    double powerConsumptions[MAX_RESISTORS] = {0};
    double voltage[MAX_RESISTORS] = {0};
    double totalVoltage = 0.0;

    char circuitType[10];
    printf("\nEnter the circuit type (SERIES or PARALLEL): ");
    scanf("%s", circuitType);

    if (strcasecmp(circuitType, "SERIES") == 0) {
        /* Series circuit analysis */
        for (int i = 0; i < resistorCount; i++) {
            totalResistance += resistors[i].value;
        }
        totalCurrent = voltageSource.value / totalResistance;
        for (int i = 0; i < resistorCount; i++) {
            currents[i] = totalCurrent;
            voltageDrops[i] = currents[i] * resistors[i].value;
            powerConsumptions[i] = currents[i] * voltageDrops[i];
            voltage[i] = voltageDrops[i];
        }
        for (int i = 0; i < resistorCount; i++) {
            totalVoltage += voltageDrops[i];
        }
    }
    else if (strcasecmp(circuitType, "PARALLEL") == 0) {
        /* Parallel circuit analysis */
        double reciprocalTotalResistance = 0.0;
        for (int i = 0; i < resistorCount; i++) {
            reciprocalTotalResistance += 1.0 / resistors[i].value;
        }
        totalResistance = 1.0 / reciprocalTotalResistance;
        totalCurrent = voltageSource.value / totalResistance;
        for (int i = 0; i < resistorCount; i++) {
            currents[i] = voltageSource.value / resistors[i].value;
            voltageDrops[i] = voltageSource.value;
            powerConsumptions[i] = currents[i] * voltageDrops[i];
            voltage[i] = voltageSource.value;
        }
        totalVoltage = voltageSource.value;
    }
    else {
        printf("Invalid circuit type.\n");
        return;
    }

    /* Print the analysis report */
    printf("\nAnalysis Report:\n");
    if (resistorCount == 3)
        printf("%10s%10s%10s%10s\n", "R1", "R2", "R3", "RT");
    else if (resistorCount == 4)
        printf("%10s%10s%10s%10s%10s\n", "R1", "R2", "R3", "R4", "RT");
    else if (resistorCount == 5)
        printf("%10s%10s%10s%10s%10s%10s\n", "R1", "R2", "R3", "R4", "R5", "RT");

    for (int i = 0; i < resistorCount; i++) {
        printf("%10.2lf ", resistors[i].value);
    }
    printf("%10.2lf\n", totalResistance);

    if (resistorCount == 3)
        printf("%10s%10s%10s%10s\n", "I1", "I2", "I3", "IT");
    else if (resistorCount == 4)
        printf("%10s%10s%10s%10s%10s\n", "I1", "I2", "I3", "I4", "IT");
    else if (resistorCount == 5)
        printf("%10s%10s%10s%10s%10s%10s\n", "I1", "I2", "I3", "I4", "I5", "IT");

    for (int i = 0; i < resistorCount; i++) {
        printf("%10.5lf ", currents[i]);
    }
    printf("%10.5lf\n", totalCurrent);

    if (resistorCount == 3)
        printf("%10s%10s%10s%10s\n", "V1", "V2", "V3", "VT");
    else if (resistorCount == 4)
        printf("%10s%10s%10s%10s%10s\n", "V1", "V2", "V3", "V4", "VT");
    else if (resistorCount == 5)
        printf("%10s%10s%10s%10s%10s%10s\n", "V1", "V2", "V3", "V4", "V5", "VT");

    for (int i = 0; i < resistorCount; i++) {
        printf("%10.5lf ", voltageDrops[i]);
    }
    printf("%10.5lf\n", totalVoltage);

    if (resistorCount == 3)
        printf("%10s%10s%10s%10s\n", "P1", "P2", "P3", "PT");
    else if (resistorCount == 4)
        printf("%10s%10s%10s%10s%10s\n", "P1", "P2", "P3", "P4", "PT");
    else if (resistorCount == 5)
        printf("%10s%10s%10s%10s%10s%10s\n", "P1", "P2", "P3", "P4", "P5", "PT");

    for (int i = 0; i < resistorCount; i++) {
        printf("%10.5lf ", powerConsumptions[i]);
    }
    printf("%10.5lf\n", totalCurrent * voltageSource.value);
}

/* Display the main menu to the user */
void displayMenu() {
    printf("\n--- Circuit Analysis & Design (CAD) Menu ---\n");
    printf("1. Create circuit (series or parallel).\n");
    printf("   - Define a new circuit by specifying voltage source and resistors.\n");
    printf("2. Load circuit (series or parallel).\n");
    printf("   - Load a saved circuit file (.cir format).\n");
    printf("3. Save circuit (must create a circuit first).\n");
    printf("   - Save the current circuit configuration to a file.\n");
    printf("4. Analyze and print report for DC analysis.\n");
    printf("   - Analyze circuit and display resistance, current, voltage, and power.\n");
    printf("5. Exit program.\n");
    printf("Please choose an option [1-5]: ");
}
