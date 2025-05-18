#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100  // Maximum number of transitions and stack size

int state = 0;  // Global state counter to assign unique state numbers

// Structure to represent a Non-deterministic Finite Automaton (NFA)
typedef struct {
    int start;  // Start state of the NFA
    int end;    // End state of the NFA
} NFA;

// Structure to represent a transition (from state -> symbol -> to state)
typedef struct {
    int from;
    char symbol;  // Symbol (can be 'a', 'b', or 'ε' for epsilon)
    int to;
} Transition;

Transition transitions[MAX];  // Array to store all transitions
int t_count = 0;              // Counter for total transitions added

NFA stack[MAX];  // Stack to hold intermediate NFA fragments
int top = -1;    // Top index for stack

// Push an NFA onto the stack
void push(NFA nfa) {
    stack[++top] = nfa;
}

// Pop an NFA from the stack
NFA pop() {
    return stack[top--];
}

// Add a transition to the global transitions list
void addTransition(int from, char symbol, int to) {
    transitions[t_count].from = from;
    transitions[t_count].symbol = symbol;
    transitions[t_count].to = to;
    t_count++;
}

// Create a basic NFA for a single character (like 'a' or 'b')
NFA create_basic_nfa(char symbol) {
    NFA nfa;
    nfa.start = state++;  // Assign a new start state
    nfa.end = state++;    // Assign a new end state
    addTransition(nfa.start, symbol, nfa.end);  // Add a transition between them
    return nfa;
}

// Create a new NFA for union operation (A|B)
NFA apply_union(NFA nfa1, NFA nfa2) {
    NFA result;
    result.start = state++;  // New start state
    result.end = state++;    // New end state

    // ε-transition from new start to both nfa1 and nfa2 starts
    addTransition(result.start, 'ε', nfa1.start);
    addTransition(result.start, 'ε', nfa2.start);

    // ε-transition from both nfa1 and nfa2 ends to new end
    addTransition(nfa1.end, 'ε', result.end);
    addTransition(nfa2.end, 'ε', result.end);

    return result;
}

// Create a new NFA for concatenation (AB)
NFA apply_concat(NFA nfa1, NFA nfa2) {
    // ε-transition from end of first NFA to start of second
    addTransition(nfa1.end, 'ε', nfa2.start);

    // Result NFA starts at nfa1.start and ends at nfa2.end
    NFA result = { nfa1.start, nfa2.end };
    return result;
}

// Create a new NFA for Kleene Star (A*)
NFA apply_kleene(NFA nfa) {
    NFA result;
    result.start = state++;  // New start state
    result.end = state++;    // New end state

    // ε-transition from new start to original NFA and to new end
    addTransition(result.start, 'ε', nfa.start);
    addTransition(result.start, 'ε', result.end);

    // ε-transitions to allow repetition and exit
    addTransition(nfa.end, 'ε', nfa.start);  // Loop
    addTransition(nfa.end, 'ε', result.end); // Exit

    return result;
}

// Construct the NFA using a postfix regular expression
void construct_nfa_from_postfix(char* postfix) {
    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (c == 'a' || c == 'b') {
            // Create and push basic NFA for character
            push(create_basic_nfa(c));
        } else if (c == '.') {
            // Concatenation: pop two NFAs and combine
            NFA nfa2 = pop();
            NFA nfa1 = pop();
            push(apply_concat(nfa1, nfa2));
        } else if (c == '|') {
            // Union: pop two NFAs and union them
            NFA nfa2 = pop();
            NFA nfa1 = pop();
            push(apply_union(nfa1, nfa2));
        } else if (c == '*') {
            // Kleene star: pop one NFA and apply star
            NFA nfa = pop();
            push(apply_kleene(nfa));
        }
    }
}

// Display the final NFA transitions
void display_nfa(NFA result) {
    printf("\nStart State: %d\nEnd State: %d\n", result.start, result.end);
    printf("Transitions:\n");

    // Print all transitions
    for (int i = 0; i < t_count; i++) {
        printf("  %d -- %c --> %d\n", transitions[i].from, transitions[i].symbol, transitions[i].to);
    }
}

// Main function
int main() {
    char postfix[50];  // To hold the postfix regular expression

    // Prompt the user for input
    printf("Enter regex in postfix (e.g., ab.|a*): ");
    scanf("%s", postfix);  // Get postfix input from user

    construct_nfa_from_postfix(postfix);  // Build the NFA

    NFA result = pop();    // The final NFA is on top of the stack
    display_nfa(result);   // Show the NFA transitions

    return 0;
}









✅ Example Regular Expression:
makefile
Copy
Edit
Infix: (a|b)*
Postfix: ab|*
🔁 Step-by-Step Evaluation (Postfix: ab|*):
We process each symbol in ab|* one by one.

▶️ Step 1: Read 'a'
Create a basic NFA:

css
Copy
Edit
state0 -- a --> state1
Push NFA(start=0, end=1) to stack

▶️ Step 2: Read 'b'
Create a basic NFA:

css
Copy
Edit
state2 -- b --> state3
Push NFA(start=2, end=3) to stack

▶️ Step 3: Read '|' (Union)
Pop two NFAs:

NFA2: start=2, end=3 (b)

NFA1: start=0, end=1 (a)

Create new states: state4 (start), state5 (end)

Add ε-transitions:

lua
Copy
Edit
state4 -- ε --> state0
state4 -- ε --> state2
state1 -- ε --> state5
state3 -- ε --> state5
Push new NFA(start=4, end=5)

▶️ Step 4: Read '*' (Kleene star)
Pop NFA: start=4, end=5

Create new states: state6 (start), state7 (end)

Add ε-transitions:

lua
Copy
Edit
state6 -- ε --> state4
state6 -- ε --> state7
state5 -- ε --> state4
state5 -- ε --> state7
Push new NFA(start=6, end=7)

🧠 Final NFA Structure:
sql
Copy
Edit
NFA start: 6
NFA end:   7

Transitions:
  0 -- a --> 1  
  2 -- b --> 3  
  4 -- ε --> 0  
  4 -- ε --> 2  
  1 -- ε --> 5  
  3 -- ε --> 5  
  6 -- ε --> 4  
  6 -- ε --> 7  
  5 -- ε --> 4  
  5 -- ε --> 7  
🖥️ Sample Input & Output:
Input:

java
Copy
Edit
Enter regex in postfix (e.g., ab.|a*): ab|*
Output:

sql
Copy
Edit
Start State: 6
End State: 7
Transitions:
  0 -- a --> 1
  2 -- b --> 3
  4 -- ε --> 0
  4 -- ε --> 2
  1 -- ε --> 5
  3 -- ε --> 5
  6 -- ε --> 4
  6 -- ε --> 7
  5 -- ε --> 4
  5 -- ε --> 7
Would you like a diagram of this NFA too?