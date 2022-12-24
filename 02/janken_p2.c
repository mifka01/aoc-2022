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

char to_you(int opponent, int end) {
  // end == 1 -> LOSE
  // end == 3 -> WIN
  // end == 2 -> DRAW
  if (end == 1) {
    if (opponent == 1)
      return opponent + 2;
    else
      return opponent - 1;
  }
  if (end == 3) {
    if (opponent == 3)
      return opponent - 2;
    else
      return opponent + 1;
  }
  return opponent;
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
      fprintf(stderr, "error: incorrect stdin format\n");
      return 1;
    }

    int end = to_number(row[2]), opponent = to_number(row[0]);
    if (end == 0 || opponent == 0) {
      fprintf(stderr, "error: unknown char on stdin\n");
      return 1;
    }

    int you = to_you(opponent, end);
    if (you == -1)
      printf("%c %c\n", row[0], row[2]);

    total_score += janken(you, opponent);
  }
  if (total_score == 0) {
    fprintf(stderr, "error: no input on stdin\n");
    return 1;
  }

  printf("Total score: %d\n", total_score);
  return 0;
}
