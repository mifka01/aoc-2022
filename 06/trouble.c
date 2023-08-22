/**
 * @title Advent of Code - 06
 * @author Mifka Radim
 **/

#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 4096
#define MARKER_LENGTH 14

int main() {

  char signal[MAX_LENGTH + 2];

  char sequence[MARKER_LENGTH + 1] = "\0";

  int passed = 0;

  fgets(signal, MAX_LENGTH + 1, stdin);

  for (int i = 0; signal[i] != '\0'; i++) {

    if (strlen(sequence) == MARKER_LENGTH) {
      passed = i;
      break;
    }

    char *found = strchr(sequence, signal[i]);

    if (found == NULL)
      strncat(sequence, &signal[i], 1);
    else {
      memmove(sequence, found + 1, MARKER_LENGTH);
      strncat(sequence, &signal[i], 1);
    }
  }
  printf("%s\n", sequence);
  printf("%d\n", passed);

  return 0;
}
