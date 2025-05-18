#include <stdio.h>      // For input/output functions like printf, scanf, fgets
#include <string.h>     // For string functions like strlen, strchr, fgets

#define MAX 100         // Maximum size for arrays

// Global declarations
char production[MAX][MAX];     // Stores the productions (e.g., E->TR)
char first[MAX][MAX];          // Stores FIRST sets of non-terminals
char follow[MAX][MAX];         // Stores FOLLOW sets of non-terminals
char nonTerminals[MAX];        // List of unique non-terminals
int ntCount = 0;               // Count of non-terminals
int prodCount = 0;             // Count of productions

// Utility function to add a character to a result string if it's not already present
void addChar(char c, char *result) {
    if(!strchr(result, c)) {                  // Check if character 'c' is already in 'result'
        result[strlen(result)] = c;           // Append 'c' at the end
        result[strlen(result) + 1] = '\0';    // Null-terminate the string
    }
}

// Checks if a character is a non-terminal (i.e., uppercase A-Z)
int isNonTerminal(int c) {
    return (c >= 65 && c <= 90);              // ASCII values for A-Z
}

// Recursive function to compute FIRST set of a given symbol 'c'
void computeFirst(char c, char *result) {
    if(!isNonTerminal(c)) {       // If terminal, add directly to result
        addChar(c, result);
        return;
    }

    for(int i = 0 ;i < prodCount; i++) {              // Loop through all productions
        if(production[i][0] == c) {                   // Match productions starting with 'c'
            char *rhs = production[i] + 3;            // RHS of production (skip 'A->')

            if(rhs[0] == '#') {                       // If production is epsilon
                addChar('#', result);                 // Add epsilon to FIRST
            } else {
                int j = 0;
                while(rhs[j]) {                       // Traverse RHS
                    char temp[MAX] = "";
                    computeFirst(rhs[j], temp);       // Recursively compute FIRST of current symbol

                    for(int k = 0; temp[k]; k++) {    // Add all symbols (except epsilon) to result
                        if(temp[k] != '#') {
                            addChar(temp[k], result);
                        }
                    }

                    if(!strchr(temp , '#')) break;    // If epsilon not in FIRST, stop
                    j++;                              // Otherwise move to next symbol
                    if(!rhs[j]) {
                        addChar('#', result);         // If all symbols can derive epsilon, add epsilon
                    }
                }
            }
        }
    }
}

// Recursive function to compute FOLLOW set of a non-terminal 'c'
void computeFollow(char c, char *result) {
    if (production[0][0] == c) {      // Start symbol gets '$' in its FOLLOW
        addChar('$', result);
    }

    for (int i = 0; i < prodCount; i++) {        // Traverse all productions
        char *rhs = production[i] + 3;           // RHS of production
        int j = 0;
        while (rhs[j]) {
            if (rhs[j] == c) {                   // Found 'c' in RHS
                if (rhs[j + 1]) {                // If there's a next symbol
                    char temp[MAX] = "";
                    computeFirst(rhs[j + 1], temp);  // Compute FIRST of next symbol

                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '#') {
                            addChar(temp[k], result); // Add FIRST (without epsilon) to FOLLOW
                        }
                    }

                    if (strchr(temp, '#') && production[i][0] != c) {
                        computeFollow(production[i][0], result); // If epsilon is in FIRST, add FOLLOW of LHS
                    }
                } else {
                    if (production[i][0] != c) {                // If 'c' is at the end, add FOLLOW of LHS
                        computeFollow(production[i][0], result);
                    }
                }
            }
            j++;
        }
    }
}

// Main function to drive the program
int main() {
    printf("Enter no.of.production: ");
    scanf("%d", &prodCount);          // Read number of productions
    getchar();                        // Consume newline left by scanf

    // Input all productions
    for(int i = 0; i < prodCount; i++) {
        printf("Enter production %d (like A->aB): ", i + 1);
        fgets(production[i], sizeof(production[i]), stdin);       // Read full line
        production[i][strcspn(production[i], "\n")] = '\0';       // Remove newline character

        // If LHS non-terminal not already recorded, add it.
        if(!strchr(nonTerminals, production[i][0])) {
            nonTerminals[ntCount++] = production[i][0];
        }
    }

    // Compute and print FIRST sets
    printf("First Sets:\n");
    for(int i = 0; i < ntCount; i++) {
        first[i][0] = '\0';                               // Initialize result string
        computeFirst(nonTerminals[i], first[i]);          // Compute FIRST for each non-terminal

        printf("FIRST(%c) = { ", nonTerminals[i]);
        for (int j = 0; first[i][j]; j++) {
            printf("%c ", first[i][j]);                   // Print each symbol in FIRST
        }
        printf("}\n");
    }

    // Compute and print FOLLOW sets
    printf("Follow Sets:\n");
    for(int i = 0; i < ntCount; i++) {
        follow[i][0] = '\0';                              // Initialize result string
        computeFollow(nonTerminals[i], follow[i]);        // Compute FOLLOW for each non-terminal

        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for (int j = 0; follow[i][j]; j++) {
            printf("%c ", follow[i][j]);                  // Print each symbol in FOLLOW
        }
        printf("}\n");
    }

    return 0;
}
