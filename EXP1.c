#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 30

// Function to check if a given string is a keyword
int is_keyword(const char *str) {
    // List of keywords to check against
    const char *keywords[] = {"int", "float", "char", ";", "if", "else", "while", "return", "for"};
    int num_keywords = 9;

    // Loop through each keyword and compare with str
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;  // It's a keyword
    }
    return 0;  // Not a keyword
}

// Function to return size in bytes of given data type
int get_data_size(const char *datatype) {
    if (strcmp(datatype, "int") == 0) return 4;
    if (strcmp(datatype, "float") == 0) return 4;
    if (strcmp(datatype, "char") == 0) return 1;
    return 0;  // Unknown type
}

int main() {
    char buffer[100];         // Input string buffer
    char c;                   // Current character being processed
    char current_datatype[10] = ""; // Stores current datatype keyword (e.g., "int", "float")
    int i = 0, n = 0;         // Loop index and length of input
    int address = 1000;       // Starting address for symbols

    printf("Enter the expression:\n");
    scanf(" %[^\n]s", buffer);  // Read entire line until newline
    n = strlen(buffer);          // Calculate length of input string

    printf("\nSymbol Table:\n");
    printf("Symbol\t\tAddress\t\tType\n");

    // Main loop: iterate through each character of input
    while (i < n) {
        c = buffer[i];

        // Skip whitespace characters (spaces, tabs, etc.)
        if (isspace(c)) {
            i++;
            continue;
        }

        // If the character is alphabetic, it could be a keyword or identifier
        if (isalpha(c)) {
            char word[20];
            int k = 0;

            // Extract entire word (alphanumeric + underscore) starting from current position
            while (isalnum(c) || c == '_') {
                word[k++] = c;
                c = buffer[++i];  // Move to next character
            }
            word[k] = '\0';  // Null-terminate the extracted word

            // Check if the word is a keyword
            if (is_keyword(word)) {
                printf("%s \t\t<-- keyword\n", word);
                // Save current datatype keyword to assign type to identifiers following this
                strcpy(current_datatype, word);
            } else {
                // If current datatype is set, this word is an identifier of that type
                if (strlen(current_datatype) > 0) {
                    printf("%s\t\t%d\t\t%s\n", word, address, current_datatype);
                    // Increment address by size of datatype for next symbol
                    address += get_data_size(current_datatype);
                } else {
                    // Otherwise, just print identifier without address or type
                    printf("%s\t\t<-- identifier\n", word);
                }
            }
        }
        // If the character is a digit, it may be a numeric constant
        else if (isdigit(c)) {
            char num[20];
            int k = 0;

            // Extract full number including decimal points
            while (isdigit(c) || c == '.') {
                num[k++] = c;
                c = buffer[++i];
            }
            num[k] = '\0';
            printf("%s \t\t<-- constant\n", num);
        }
        // Check if character is an operator
        else if (strchr("+-*/%=", c)) {
            printf("%c \t\t<-- operator\n", c);
            i++;
        }
        // Check for separators like parentheses, braces, brackets
        else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
            printf("%c \t\t<-- separator\n", c);
            i++;
        }
        // Check for statement terminator ';'
        else if (c == ';') {
            printf("%c \t\t<-- terminator\n", c);
            i++;
        }
        else {
            // For any other character, just move forward
            i++;
        }
    }

    return 0;
}
