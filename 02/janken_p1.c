/**
 * @title Advent of Code - 02
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 3

int to_number(char c) {
  if (c == 'A' || c == 'X')
    return 1;

  if (c == 'B' || c == 'Y')
    return 2;

  if (c == 'C' || c == 'Z')
    return 3;
  return 0;
}

int janken(int you, int opponent) {

  if (you == opponent - 1 || opponent == you - 2)
    return you;

  if (you == opponent - 2 || opponent == you - 1)
    return you + 6;

  return you + 3;
}

int main() {

  char row[MAX_LENGTH + 2];
  unsigned int total_score = 0;

  for (size_t i = 1; fgets(row, MAX_LENGTH + 2, stdin) != NULL; i++) {
    if (row[3] != '\n') {
      fprintf(stderr, "error: incorrect stdin format");
      return 1;
    }
    int you = to_number(row[2]), opponent = to_number(row[0]);

    if (you == 0 || opponent == 0) {
      fprintf(stderr, "error: unknown char on stdin");
      return 1;
    }
    total_score += janken(you, opponent);
  }

  printf("%d\n", total_score);
  return 0;
}
