#include <stdbool.h>
#include <stdio.h>  // Standard input/output library
#include <stdlib.h> // Standard library for general functions
#include <string.h> // String manipulation functions
#include <dirent.h> // Directory entry for access
/**
 * A simple shell program that accepts and processes basic commands
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit status code
 */
int main(int argc, char *argv[]) {
  while (true) {\
    // Disable output buffering to ensure prompt appears immediately
    setbuf(stdout, NULL);
    // Display shell prompt
    printf("$ ");
    char *builtins[] = {"exit", "type", "echo"};
    // Read user input (maximum 99 characters plus null terminator)
    char input[100];
    fgets(input, 100, stdin);
    // Remove trailing newline character from input
    input[strcspn(input, "\n")] = 0;
    // Check if user wants to exit the shell
    if (strcmp(input, "exit 0") == 0) {
      break; // Exit the loop if user types "exit 0"
    } 
    else if (strstr(input, "type")) {
      // Extract the command ("echo")
      char *cmd = strtok(input, " "); 
      // Extract everything after the first space
      char *message = strtok(NULL, "\0"); 
      // this is the flag to kick out depending on what if the command is found or not.
      bool flag = false;
      // roll through the builtins and check if it matchs (see line 18)
      for (int i = 0; i < sizeof(builtins) / sizeof(char *); i++) {
        // if the message matches one of the builtins strcmp will return 0
        if (!strcmp(message, builtins[i])) {
          // flag is true, so print the message but not the fail message
          flag = true;
          printf("%s is a shell builtin\n", builtins[i]);
          break;
        } 
        else continue;
      }
      // if the flag is true, then bypass the rest of the code otherwise go on and 
      // search for the file in the PATH
      if (flag) continue;
      struct dirent *de; // Pointer for directory entry 
      // opendir() returns a pointer of DIR type. 
      DIR *dr;
      char* path = getenv("PATH");
      if (path != NULL) {
        char path_copy[1024]; // Create a copy buffer (ensure it's large enough)
        // Copy the PATH value because
        // path is permanently modified by strtok
        strncpy(path_copy, path, sizeof(path_copy) - 1); 
        path_copy[sizeof(path_copy) - 1] = '\0'; // Null-terminate the string

        char* tok = strtok(path_copy, ":");
        while (tok != NULL) {
          dr = opendir(tok); 
          if (dr != NULL) {
            while ((de = readdir(dr)) != NULL) {
              if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
              else if (!strcmp(message, de->d_name)) {
                flag = true;
                printf("%s is %s/%s\n", de->d_name, tok, de->d_name);
                break;
              }
            }
            closedir(dr);
            if (flag) break;
          }
          tok = strtok(NULL, ":");
        }
      }
      if (!flag) printf("%s: not found\n", message);
    }
    // Handle echo command - prints the text after "echo"
    else if (strstr(input, "echo")) {
      // Split the input into command and message parts
      char *cmd = strtok(input, " "); // Extract the command ("echo")
      char *message = strtok(NULL, "\0"); // Extract everything after the first space
      // Print the message part
      printf("%s\n", message);
    }
    // Handle unrecognized commands
    else {
      printf("%s: command not found\n", input);
    }
  }
  return 0; // Return success status code
}