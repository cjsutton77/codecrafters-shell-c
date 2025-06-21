#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void quotes(char*, int, bool);
void backslash(char*, char*, int);
void handle_echo(const char *input);

int main(int argc, char *argv[]) {
    while (true) {
        setbuf(stdout, NULL);
        printf("$ ");
        char input[100];
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit 0") == 0) break;

        if (strncmp(input, "type ", 5) == 0) {
            char *message = input + 5;
            char *builtins[] = {"cd", "exit", "type", "echo", "pwd"};
            bool flag = false;
            for (int i = 0; i < sizeof(builtins) / sizeof(char *); i++) {
                if (strcmp(message, builtins[i]) == 0) {
                    printf("%s is a shell builtin\n", builtins[i]);
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                // Search PATH for external command
                struct dirent *de;
                DIR *dr;
                char *path = getenv("PATH");
                if (path) {
                    char path_copy[1024];
                    strncpy(path_copy, path, sizeof(path_copy) - 1);
                    path_copy[sizeof(path_copy) - 1] = '\0';
                    char *tok = strtok(path_copy, ":");
                    while (tok) {
                        dr = opendir(tok);
                        if (dr) {
                            while ((de = readdir(dr)) != NULL) {
                                if (strcmp(de->d_name, message) == 0) {
                                    printf("%s is %s/%s\n", de->d_name, tok, de->d_name);
                                    flag = true;
                                    break;
                                }
                            }
                            closedir(dr);
                            if (flag) break;
                        }
                        tok = strtok(NULL, ":");
                    }
                }
            }
            if (!flag) printf("%s: not found\n", message);
        }
        else if (strcmp(input, "pwd") == 0) {
            char *pwd = getenv("PWD");
            if (pwd) {
                int len = strlen(pwd);
                if (len > 0 && pwd[len - 1] == '/') pwd[len - 1] = '\0';
                printf("%s\n", pwd);
            }
        }
        else if (strncmp(input, "cd", 2) == 0 && (input[2] == ' ' || input[2] == '\0')) {
            char *home = getenv("HOME");
            char *pwd = getenv("PWD");
            char *message = NULL;
            if (input[2] == ' ') message = input + 3;
            if (!message || strcmp(message, "~") == 0 || strlen(message) == 0) {
                chdir(home);
                setenv("PWD", home, 1);
            } else {
                int retval = chdir(message);
                if (retval != 0) {
                    printf("cd: %s: No such file or directory\n", message);
                } else {
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    setenv("PWD", cwd, 1);
                }
            }
        }
        else if (strncmp(input, "echo", 4) == 0 && (input[4] == ' ' || input[4] == '\0')) {
            handle_echo(input);
        }
        else {
            // External command execution
            char input_copy[1024];
            strncpy(input_copy, input, sizeof(input_copy) - 1);
            input_copy[sizeof(input_copy) - 1] = '\0';
            char *cmd = strtok(input_copy, " ");
            bool flag = false;
            char *path = getenv("PATH");
            if (path) {
                char path_copy[1024];
                strncpy(path_copy, path, sizeof(path_copy) - 1);
                path_copy[sizeof(path_copy) - 1] = '\0';
                char *tok = strtok(path_copy, ":");
                while (tok) {
                    DIR *dr = opendir(tok);
                    if (dr) {
                        struct dirent *de;
                        while ((de = readdir(dr)) != NULL) {
                            if (strcmp(cmd, de->d_name) == 0) {
                                flag = true;
                                system(input);
                                break;
                            }
                        }
                        closedir(dr);
                        if (flag) break;
                    }
                    tok = strtok(NULL, ":");
                }
            }
            if (!flag) printf("%s: command not found\n", input);
        }
    }
    return 0;
}

void handle_echo(const char *input) {
    system(input);
    // Skip "echo" and any following spaces
    // const char *msg = input + 4;
    // while (*msg == ' ') msg++;

    // if (*msg == '\'') {
    //     // Single-quoted
    //     printf("!HI!\n");
    //     msg++;
    //     const char *end = strchr(msg, '\'');
    //     if (end) {
    //         fwrite(msg, 1, end - msg, stdout);
    //         putchar('\n');
    //     } else {
    //         printf("\n");
    //     }
    // } else if (*msg == '"') {
    //     // Double-quoted
    //     msg++;
    //     const char *end = strchr(msg, '"');
    //     if (end) {
    //         fwrite(msg, 1, end - msg, stdout);
    //         putchar('\n');
    //     } else {
    //         printf("\n");
    //     }
    // } else if (strchr(msg, '\\')) {
    //     // Backslash handling
    //     char newMessage[1024];
    //     backslash((char*)msg, newMessage, strlen(msg));
    // } else {
        // Plain echo
        // printf("%s\n", msg);
    // }
}

void quotes(char* wordsFrom, int length, bool single) {
  int counter = 0;
  bool inside=false;
  char prev = '\0';
  char next = '\0';
  char wordsTo[1024] = {};
  if (single){
    for (int i = 0; i < length; i++){
      if (wordsFrom[i] == '\''){
        continue;
      }
      else {
        wordsTo[counter++] = wordsFrom[i];
      }
    }
    printf("%s\n",wordsTo);
  }
  else {
    for (int i = 0; i < length; i++){
      if (i > 0) {
        prev = wordsFrom[i-1];
        next = wordsFrom[i];
      }
      if (wordsFrom[i] == '\"'){
        inside = !inside;
        continue;
      }
      else {
        if (inside)
        {
          wordsTo[counter++] = wordsFrom[i];
        }
        else if ((prev == ' ') && (next == ' ')){
          wordsTo[counter++] = ' ';
          //continue;
        }
        
      }
    }
    printf("%s\n",wordsTo);
  }
}

void backslash(char* oldMessage, char* newMessage, int length){
  char prev = oldMessage[0];
  char next = oldMessage[1];
  int counter = 0;
  int start = 1;
  if (prev == '\\' && next == '\''){
    newMessage[counter++] = '\'';
    ++start;
  }
  else if (prev == '\\' && next == '\"'){
    newMessage[counter++] = '\"';
    ++start;
  }
  else {
    newMessage[counter++] = oldMessage[0];
  }
  for (int i = start; i < length+1; i++){
    //printf(".");
    prev = oldMessage[i-1];
    next = oldMessage[i];
    if (next == '\\'){
      continue;
    }
    // else if (prev == ' ' && next == ' '){
    //   newMessage[counter++] = ' ';
    // }
    else if (prev == '\\' && next == ' '){
      newMessage[counter++] = ' ';
    }
    else if (prev == '\\' && next == '\''){
      newMessage[counter++] = '\'';
    }
    else if (prev == '\\' && next == '\"'){
      newMessage[counter++] = '\"';
    }
    else {
      newMessage[counter++] = oldMessage[i];
    }
  }
  printf("%s\n",newMessage);
}
