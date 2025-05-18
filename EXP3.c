#include <stdio.h>      // For input and output functions
#include <stdlib.h>     // For general utility functions
#include <stdbool.h>    // For using boolean type (true/false)

#define MAX_STATES 100         // Maximum number of states allowed
#define MAX_TRANSITIONS 100    // Maximum number of transitions allowed

// Structure to represent a transition in the NFA
typedef struct {
    int start;     // Start state of the transition
    int end;       // End state of the transition
    char symbol;   // Transition symbol (use '#' for epsilon)
} Transition;

// Structure to represent an NFA
typedef struct {
    int states;                      // Total number of states
    int transition_count;           // Number of transitions
    Transition transitions[MAX_TRANSITIONS]; // Array of transitions
} NFA;

// Function to initialize all visited states to false
void initialize_visited(bool visited[], int size) {
    for (int i = 0; i < size; i++) {
        visited[i] = false;
    }
}

// Recursive function to compute epsilon-closure from a given state
void epsilon_closure_recursive(NFA *nfa, int state, bool visited[]) {
    visited[state] = true;  // Mark current state as visited

    // Traverse all transitions to find epsilon transitions from current state
    for (int i = 0; i < nfa->transition_count; i++) {
        if (nfa->transitions[i].start == state && nfa->transitions[i].symbol == '#') {
            // If epsilon transition exists and end state is not visited, recurse
            if (!visited[nfa->transitions[i].end]) {
                epsilon_closure_recursive(nfa, nfa->transitions[i].end, visited);
            }
        }
    }
}

// Function to compute and print epsilon-closure of each state in the NFA
void compute_epsilon_closure(NFA *nfa) {
    printf("Epsilon closure of states in NFA:\n");

    // Loop through each state in the NFA
    for (int i = 0; i < nfa->states; i++) {
        bool visited[MAX_STATES];               // To keep track of visited states
        initialize_visited(visited, MAX_STATES); // Initialize all as unvisited

        epsilon_closure_recursive(nfa, i, visited); // Compute epsilon-closure for state i

        // Print the closure result
        printf("ε-closure(%d) : { ", i);
        for (int j = 0; j < nfa->states; j++) {
            if (visited[j]) {
                printf("%d ", j); // Print all states reachable via epsilon from state i
            }
        }
        printf("}\n");
    }
}

// Main function
int main() {
    NFA nfa;

    // Read number of states in the NFA
    printf("Enter no. of states in the NFA: ");
    scanf("%d", &nfa.states);

    // Read number of transitions
    printf("Enter no. of transitions in the NFA: ");
    scanf("%d", &nfa.transition_count);

    // Read all transitions from user
    printf("Enter transitions (start_state end_state symbol):\n");
    for (int i = 0; i < nfa.transition_count; i++) {
        scanf("%d %d %c", &nfa.transitions[i].start, &nfa.transitions[i].end, &nfa.transitions[i].symbol);
    }

    // Compute epsilon-closure for all states
    compute_epsilon_closure(&nfa);

    return 0;
}





