#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void cmd_help(void);
void cmd_exit(void);
typedef void (*command_func)(void);

typedef struct {
  char *name;
  command_func func;
} Command;

void cmd_help(void) { printf("Available commands: help,exit\n"); }

void cmd_exit(void) {
  printf("Exiting...\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  Command commands[] = {{"help", cmd_help}, {"exit", cmd_exit}};
  char input[100];
  while (1) {
    printf("> ");
    scanf("%s", input);
  }
  int found = 0;
  for (int i = 0; i < sizeof(commands) / sizeof(Command); i++) {
    if (strcmp(input, commands[i].name) == 0) {
      commands[i].func();
      found = 1;
      break;
    }
  }
  if (!found) {
    printf("Unknown command: %s\n", input);
  }
  return 0;
}
