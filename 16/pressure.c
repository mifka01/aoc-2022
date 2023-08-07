/**
 * @title Advent of Code - 16
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <iso646.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 500
#define REALLOC_AMOUNT 10

int main() {
  char row[MAX_LENGTH + 2];

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;
  }

  return 0;
}
