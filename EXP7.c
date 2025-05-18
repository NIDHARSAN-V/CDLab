#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void optimizeStrengthReduction(char* expr) {
    char var[10], op1[10], op2[10];
    if (sscanf(expr, "%s = %s * %s", var, op1, op2) == 3) {
        if (strcmp(op1, var) == 0 && strcmp(op2, "2") == 0) {
            sprintf(expr, "%s = %s + %s", var, op1, op1);
            printf("Optimized (strength reduction): %s\n", expr);
            return;
        }
    }
    printf("No strength reduction applied.\n");
}

int optimize(char* expr) {
    char var[10], op1[10], op2[10];
    if (sscanf(expr, "%s = %s + %s", var, op1, op2) == 3) {
        if (strcmp(var, op1) == 0 && strcmp(op2, "0") == 0) {
            sprintf(expr, "%s = %s", var, op1);
            printf("Optimized (add zero removal): %s\n", expr);
            return 1;
        }
    }
    else if (sscanf(expr, "%s = %s * %s", var, op1, op2) == 3) {
        if (strcmp(var, op1) == 0 && strcmp(op2, "1") == 0) {
            sprintf(expr, "%s = %s", var, op1);
            printf("Optimized: %s\n", expr);
            return 1;
        }
    }
    return 0;
}

int optimizeConstantFoldOperator(char* expr) {
    int lhs, rhs;
    char var[10];
    char op;

    if (sscanf(expr, "%s = %d %c %d", var, &lhs, &op, &rhs) == 4) {
        int result;
        switch (op) {
            case '+': result = lhs + rhs; break;
            case '-': result = lhs - rhs; break;
            case '*': result = lhs * rhs; break;
            case '/':
                if (rhs == 0) {
                    printf("Error: Division by zero\n");
                    return 0;
                }
                result = lhs / rhs;
                break;
            default:
                return 0;
        }
        sprintf(expr, "%s = %d", var, result);
        printf("Optimized (constant folding): %s\n", expr);
        return 1;
    }
    return 0;
}

int main() {
    char expr[100];

    printf("Enter expressions to optimize (type 'exit' to quit):\n");

    while (1) {
        printf("\n> ");
        fgets(expr, sizeof(expr), stdin);

        expr[strcspn(expr, "\n")] = '\0';

        if (strcmp(expr, "exit") == 0) {
            break;
        }

        if (optimizeConstantFoldOperator(expr)) continue;
        if (optimize(expr)) continue;
        optimizeStrengthReduction(expr);
    }

    return 0;
}
                