#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int tempCount = 1;

// Function to generate a new temporary variable like t1, t2, ...
char* newTemp() {
    static char temp[10];
    sprintf(temp, "t%d", tempCount++);
    return strdup(temp); // strdup duplicates the string for safe stack use
}

// Operand stack
char* strStack[MAX];
int topStr = -1;

// Push operand onto operand stack
void pushStr(char* s) {
    strStack[++topStr] = strdup(s); // Push string (operand) onto stack
}

// Pop operand from operand stack
char* popStr() {
    return strStack[topStr--];
}

// Operator stack
char opStack[MAX];
int topOp = -1;

// Push operator onto stack
void pushOp(char c) {
    opStack[++topOp] = c;
}

// Pop operator from stack
char popOp() {
    return opStack[topOp--];
}

// Peek top of operator stack
char peekOp() {
    return opStack[topOp];
}

// Check if operator stack is empty
int isEmptyOp() {
    return topOp == -1;
}

// Define precedence of operators
int precedence(char op) {
    if (op == '~') return 3;               // Unary minus has highest precedence
    if (op == '*' || op == '/') return 2;  // Multiplication and division
    if (op == '+' || op == '-') return 1;  // Addition and subtraction
    return 0;
}

// Check if a character is a valid operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '~';
}

// Apply an operator by popping operands from the stack and pushing result
void applyOp(char op) {
    if (op == '~') {
        // Unary minus: apply to one operand
        char* operand = popStr();
        char* temp = newTemp();
        printf("%s = uminus %s\n", temp, operand); // Generate TAC
        pushStr(temp); // Push result back
    } else {
        // Binary operators: pop two operands
        char* right = popStr();
        char* left = popStr();
        char* temp = newTemp();
        printf("%s = %s %c %s\n", temp, left, op, right); // Generate TAC
        pushStr(temp); // Push result back
    }
}

// Generate Three Address Code from an expression
void generateTAC(char* expr) {
    for (int i = 0; expr[i]; i++) {
        char c = expr[i];

        if (c == ' ') continue;

        if (isalnum(c)) {
            char var[2] = {c, '\0'};
            pushStr(var);
        } 
        else if (c == '(') {
            pushOp(c);
        } 
        else if (c == ')') {
            while (peekOp() != '(') applyOp(popOp());
            popOp(); // pop '('
        } 
        else if (isOperator(c)) {
            // Check for unary minus
            if (c == '-' && (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1]))) {
                pushOp('~'); // unary minus
            } else {
                while (!isEmptyOp() && precedence(peekOp()) >= precedence(c))
                    applyOp(popOp());
                pushOp(c);
            }
        }
    }

    while (!isEmptyOp()) applyOp(popOp());
}


// Main processing function
void process(char* input) {
    char lhs, rhs[MAX];
    sscanf(input, "%c = %[^\n]", &lhs, rhs); // Split input into LHS and RHS

    printf("Input Expression: %s\n", input);
    printf("Three Address Code:\n");
    generateTAC(rhs); // Generate TAC for RHS
    char* result = popStr(); // Final result of expression
    printf("%c = %s\n", lhs, result); // Assign to LHS
}

// Main function
int main() {
    char expr[MAX];

    printf("Enter an expression (e.g., a = b * -c + b * -c):\n");
    fgets(expr, MAX, stdin); // Read full line
    process(expr); // Process input

    return 0;
}
