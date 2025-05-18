#include <stdio.h>
#include <string.h>
#define MAX 100

char production[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
char nonTerminals[MAX];
char terminals[MAX];
char parsingTable[MAX][MAX][MAX];

int ntCount = 0;
int termCount = 0;
int prodCount = 0;

void addChar(char c, char *result) {
    if (!strchr(result, c)) {
        int len = strlen(result);
        result[len] = c;
        result[len + 1] = '\0';
    }
}

void addTerminal(char c) {
    if (!strchr(terminals, c) && c != '#' && !(c >= 'A' && c <= 'Z')) {
        terminals[termCount++] = c;
    }
}

int isNonTerminal(int c) {
    return (c >= 65 && c <= 90);
}

void computeFirst(char c, char *result) {
    if (!isNonTerminal(c)) {
        addChar(c, result);
        addTerminal(c); // collect terminals
        return;
    }

    for (int i = 0; i < prodCount; i++) {
        if (production[i][0] == c) {
            char *rhs = production[i] + 3;
            if (rhs[0] == '#') {
                addChar('#', result);
            } else {
                int j = 0;
                while (rhs[j]) {
                    char temp[MAX] = "";
                    computeFirst(rhs[j], temp);

                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '#') {
                            addChar(temp[k], result);
                        }
                    }

                    if (!strchr(temp, '#'))
                        break;

                    j++;
                    if (!rhs[j]) {
                        addChar('#', result);
                    }
                }
            }
        }
    }
}

void computeFollow(char c, char *result) {
    if (production[0][0] == c) {
        addChar('$', result);
    }

    for (int i = 0; i < prodCount; i++) {
        char *rhs = production[i] + 3;
        int j = 0;
        while (rhs[j]) {
            if (rhs[j] == c) {
                if (rhs[j + 1]) {
                    char temp[MAX] = "";
                    computeFirst(rhs[j + 1], temp);

                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '#') {
                            addChar(temp[k], result);
                        }
                    }

                    if (strchr(temp, '#') && production[i][0] != c) {
                        computeFollow(production[i][0], result);
                    }
                } else {
                    if (production[i][0] != c) {
                        computeFollow(production[i][0], result);
                    }
                }
            }
            j++;
        }
    }
}

void createParsingTable() {
    for (int i = 0; i < prodCount; i++) {
        char nt = production[i][0];
        char *rhs = production[i] + 3;
        char tempFirst[MAX] = "";
        computeFirst(rhs[0], tempFirst);

        for (int k = 0; tempFirst[k]; k++) {
            if (tempFirst[k] != '#') {
                int row = strchr(nonTerminals, nt) - nonTerminals;
                int col = strchr(terminals, tempFirst[k]) - terminals;
                strcpy(parsingTable[row][col], production[i]);
            }
        }

        if (strchr(tempFirst, '#')) {
            char tempFollow[MAX] = "";
            computeFollow(nt, tempFollow);
            for (int k = 0; tempFollow[k]; k++) {
                int row = strchr(nonTerminals, nt) - nonTerminals;
                int col = strchr(terminals, tempFollow[k]) - terminals;
                strcpy(parsingTable[row][col], production[i]);
            }
        }
    }
}



void displayParsingTable() {
    printf("\nPredictive Parsing Table:\n\n");
    printf("%10s", "");
    for (int j = 0; j < termCount; j++) {
        printf("%10c", terminals[j]);
    }
    printf("\n");

    for (int i = 0; i < ntCount; i++) {
        printf("%10c", nonTerminals[i]);
        for (int j = 0; j < termCount; j++) {
            if (strlen(parsingTable[i][j]) > 0)
                printf("%10s", parsingTable[i][j]);
            else
                printf("%10s", "-");
        }
        printf("\n");
    }
}




int main() {
    printf("Enter no.of.production: ");
    scanf("%d", &prodCount);
    getchar();

    for (int i = 0; i < prodCount; i++) {
        printf("Enter production %d (like A->aB): ", i + 1);
        fgets(production[i], sizeof(production[i]), stdin);
        production[i][strcspn(production[i], "\n")] = '\0';

        if (!strchr(nonTerminals, production[i][0])) {
            nonTerminals[ntCount++] = production[i][0];
        }
    }

    printf("First Sets:\n");
    for (int i = 0; i < ntCount; i++) {
        first[i][0] = '\0';
        computeFirst(nonTerminals[i], first[i]);
        printf("FIRST(%c) = { ", nonTerminals[i]);
        for (int j = 0; first[i][j]; j++) {
            printf("%c ", first[i][j]);
        }
        printf("}\n");
    }

    printf("Follow Sets:\n");
    for (int i = 0; i < ntCount; i++) {
        follow[i][0] = '\0';
        computeFollow(nonTerminals[i], follow[i]);
        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for (int j = 0; follow[i][j]; j++) {
            printf("%c ", follow[i][j]);
        }
        printf("}\n");
    }

    addTerminal('$');
    createParsingTable();
    displayParsingTable();

    return 0;
}
            