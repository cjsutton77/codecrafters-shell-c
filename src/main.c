#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  while(true){
    // Flush after every printf
    setbuf(stdout, NULL);

    // Uncomment this block to pass the first stage
    printf("$ ");

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "exit 0") == 0){
      break;
    }
    else {
    printf("%s: command not found\n", input);
    }
  }
  return 0;
}
