    #include <stdio.h>
    

    typedef struct {
        char result[5];
        char arg1;
        char op;
        char arg2;
    } TAC;




    
    void generateAssembly(TAC tac[], int count) {
        printf("\nAssembly Code:\n");
        for (int i = 0; i < count; i++) {
            printf("%s := %c %c %c\n", tac[i].result, tac[i].arg1, tac[i].op, tac[i].arg2);
            printf("MOV R0, %c\n", tac[i].arg1);
            switch (tac[i].op) {
                case '+': printf("ADD R0, %c\n", tac[i].arg2); break;
                case '-': printf("SUB R0, %c\n", tac[i].arg2); break;
                case '*': printf("MUL R0, %c\n", tac[i].arg2); break;
                case '/': printf("DIV R0, %c\n", tac[i].arg2); break;
            }
            printf("MOV %s, R0\n\n", tac[i].result);
        }
    }



    

    
    int main() {
        int count;
        TAC tac[50];
    
        printf("Enter number of TAC instructions: ");
        scanf("%d", &count);
    
        printf("Enter TAC in the form (result := arg1 op arg2):\n");
        for (int i = 0; i < count; i++) {
            scanf(" %s := %c %c %c", tac[i].result, &tac[i].arg1, &tac[i].op, &tac[i].arg2);
        }
    
        generateAssembly(tac, count);
    
        return 0;
    }
    
                