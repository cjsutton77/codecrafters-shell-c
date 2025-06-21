#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>



// Shell command structure
typedef struct {
    const char *name;
    void (*handler)(char*);
} Command;

// Function declarations
void handle_cd(char *args);
void handle_pwd(char *args);
void handle_echo(char *input);
void handle_type(char *args);
void handle_exit(char *args);
void quotes(char *input, int length, bool single);
void backslash(char *oldMessage, char *newMessage, int length);
bool execute_external_command(char *input);

#endif
