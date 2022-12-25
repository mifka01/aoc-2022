
/**
 * @title Advent of Code - 03
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define NUM_COMPARTMENTS 2
#define ALPHABET_LENGHT 26
#define GROUP_SIZE 3

typedef struct {
  char items[GROUP_SIZE][MAX_LENGTH + 2];
} t_group;

int to_priority(char c) {
  // a-z -> 1-26
  // A-Z -> 27-52
  if (c >= 'A' && c <= 'Z')
    return c - 'A' + 1 + ALPHABET_LENGHT;

  if (c >= 'a' && c <= 'z')
    return c - 'a' + 1;

  fprintf(stderr, "erorr: not a letter");
  return -1;
}

int get_badge_priority(t_group group) {

  for (int i = 0; i < group.items[0][i] != '\0'; i++) {
    char c = group.items[0][i];
    int badge = 1;

    for (int j = 0; j < GROUP_SIZE; j++) {
      if (strchr(group.items[j], c) == NULL) {
        badge = 0;
        break;
      }
    }
    if (badge)
      return to_priority(c);
  }
  return 0;
}

int main() {

  int total_priority = 0;
  t_group group;
  int i = 0;

  while (fgets(group.items[i], MAX_LENGTH + 2, stdin) != NULL) {

    if ((i + 1) % GROUP_SIZE == 0) {
      int badge_priority = get_badge_priority(group);
      if (badge_priority == 0)
        return 1;

      total_priority += badge_priority;
      i = 0;
    } else
      i++;
  }

  printf("Total priorities of badges is: %d\n", total_priority);
  return 0;
}
