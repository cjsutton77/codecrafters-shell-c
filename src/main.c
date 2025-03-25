#include <stdio.h>   // Standard input/output library
#include <stdlib.h>  // Standard library for general functions
#include <string.h>  // String manipulation functions
#include <stdbool.h>

/**
 * A simple shell program that accepts and processes basic commands
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit status code
 */
int main(int argc, char *argv[]) {
    while(true) {
        // Disable output buffering to ensure prompt appears immediately
        setbuf(stdout, NULL);
        
        // Display shell prompt
        printf("$ ");
        
        char* builtins[] = {"exit","echo","type"};

        // Read user input (maximum 99 characters plus null terminator)
        char input[100];
        fgets(input, 100, stdin);
        
        // Remove trailing newline character from input
        input[strcspn(input, "\n")] = 0;
        
        // Check if user wants to exit the shell
        if (strcmp(input, "exit 0") == 0) {
            break;  // Exit the loop if user types "exit 0"
        }
        // Handle echo command - prints the text after "echo"
        else if (strstr(input, "echo")) {
            // Split the input into command and message parts
            char* cmd = strtok(input, " ");       // Extract the command ("echo")
            char* message = strtok(NULL, "\0");   // Extract everything after the first space
            
            // Print the message part
            printf("%s\n", message);
        }
        else if (strstr(input, "type")){
            char* cmd = strtok(input, " ");       // Extract the command ("echo")
            char* message = strtok(NULL, "\0");   // Extract everything after the first space
            bool flag = true;
            for (int i = 0; i < sizeof(builtins)/sizeof(char*); i++){
                if (!strcmp(message,builtins[i])){
                    flag = false;
                    printf("%s is a shell builtin\n",message);
                }
                else continue;
            }
            if (flag) printf("%s: not found\n",message);
        }
        // Handle unrecognized commands
        else {
            printf("%s: command not found\n", input);
        }
    }
    
    return 0;  // Return success status code
}