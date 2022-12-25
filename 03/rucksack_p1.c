
/**
 * @title Advent of Code - 02
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

typedef struct {
  char items[MAX_LENGTH / 2];
  int size;

} t_compartment;

typedef struct {
  t_compartment compartments[NUM_COMPARTMENTS];
} t_rucksack;

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

int get_duplicate_items(t_compartment fcompartment,
                        t_compartment scompartment) {

  int total_priority = 0;

  for (int i = 0; i < fcompartment.size; i++) {
    if (strchr(scompartment.items, fcompartment.items[i]) &&
        strchr(fcompartment.items, fcompartment.items[i]) ==
            &fcompartment.items[i]) {
      total_priority += to_priority(fcompartment.items[i]);
    }
  }

  return total_priority;
}

int get_duplicates_priority(t_rucksack rucksack) {
  int total_priority = 0;
  for (size_t i = 0; i < NUM_COMPARTMENTS; i++) {
    for (size_t j = i + 1; j < NUM_COMPARTMENTS; j++) {
      int duplicates_priority = get_duplicate_items(rucksack.compartments[i],
                                                    rucksack.compartments[j]);
      if (duplicates_priority == -1) {
        return -1;
      }
      total_priority += duplicates_priority;
    }
  }
  return total_priority;
}

t_rucksack create_rucksack(char *items) {

  t_rucksack rucksack;
  int compartment_len = strlen(items) / NUM_COMPARTMENTS;

  for (size_t i = 0; i < NUM_COMPARTMENTS; i++) {
    strncpy(rucksack.compartments[i].items, &items[i * compartment_len],
            compartment_len);
    rucksack.compartments[i].size = compartment_len;
    rucksack.compartments[i].items[compartment_len] = '\0';
  }

  return rucksack;
}
int main() {

  char items[MAX_LENGTH + 2];
  int total_priority = 0;

  for (size_t i = 1; fgets(items, MAX_LENGTH + 2, stdin) != NULL; i++) {
    t_rucksack rucksack = create_rucksack(items);
    int rucksack_duplicates_priority = get_duplicates_priority(rucksack);
    if (rucksack_duplicates_priority == -1) {
      return -1;
    }
    total_priority += rucksack_duplicates_priority;
  }

  printf("Total priorities of duplicate items: %d\n", total_priority);
  return 0;
}
