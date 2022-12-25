
/**
 * @title Advent of Code - 04
 * @author Mifka Radim
 **/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

typedef struct {
  int first_section;
  int last_section;
} t_elf;

int section_overlaps(t_elf first_elf, t_elf second_elf) {

  if (first_elf.first_section <= second_elf.first_section &&
      first_elf.last_section >= second_elf.last_section) {
    return 1;
  }

  if (first_elf.first_section >= second_elf.first_section &&
      first_elf.last_section <= second_elf.last_section) {
    return 1;
  }

  return 0;
}

int main() {

  char row[MAX_LENGTH + 2];
  int overlaps = 0;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    t_elf first_elf;
    t_elf second_elf;
    if (sscanf(row, "%d-%d,%d-%d", &first_elf.first_section,
               &first_elf.last_section, &second_elf.first_section,
               &second_elf.last_section) == 0) {
      return 1;
    }
    overlaps += section_overlaps(first_elf, second_elf);
  }

  printf("Total overlaps: %d\n", overlaps);

  return 0;
}
