/**
 * @title Advent of Code - 01
 * @author Mifka Radim
 **/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 102

typedef struct elf {
  unsigned int id;
  unsigned int callories;

} t_elf;

void print_top_elves(t_elf *top, unsigned int num_top_elves) {
  unsigned int callories = 0;
  printf("TOP %d elves:\n", num_top_elves);
  for (unsigned int i = 0; i != num_top_elves; i++) {
    printf("%d. Elf %d. is carrying %d Calories\n", i + 1, top[i].id,
           top[i].callories);
    callories += top[i].callories;
  }
  printf("Top elves carrying total of: %d Calories\n", callories);
}

void insert_elf(t_elf *top, t_elf elf, unsigned int insert_at,
                unsigned int num_top_elves) {
  for (size_t i = num_top_elves; insert_at < i; i--) {
    top[i] = top[i - 1];
  }
  top[insert_at] = elf;
}

void set_max(t_elf *top, t_elf elf, unsigned int num_top_elves) {
  if (elf.id + 1 < num_top_elves) {
    insert_elf(top, elf, elf.id + 1, num_top_elves);
    return;
  }

  for (size_t i = 0; i < num_top_elves; i++) {
    if (top[i].callories < elf.callories) {
      insert_elf(top, elf, i, num_top_elves);
      break;
    }
  }
}

int read_arguments(int argc, char **argv, unsigned int *num_top_elves) {
  if (argc != 2) {
    return 1;
  }

  char *pEnd;
  unsigned long int converted = strtol(argv[1], &pEnd, 10);

  if (converted <= 0 || converted > INT_MAX || strlen(pEnd) != 0) {
    fprintf(stderr, "error: value to 'N' is invalid (expected "
                    "'positive int')\n");
    return 1;
  }

  *num_top_elves = converted;
  return 0;
}

int main(int argc, char **argv) {

  unsigned int num_top_elves;

  if (read_arguments(argc, argv, &num_top_elves)) {
    fprintf(stderr, "usage: mcal N\n");
    return 1;
  }

  char row[MAX_LENGTH + 1];
  t_elf top[num_top_elves];

  t_elf elf = {.id = 0, .callories = 0};

  for (size_t i = 1; fgets(row, MAX_LENGTH + 1, stdin) != NULL; i++) {
    if (row[0] == '\n') {
      set_max(top, elf, num_top_elves);

      elf.callories = 0;
      elf.id++;

    } else {
      unsigned long converted = strtol(row, (char **)NULL, 10);
      if (!converted) {
        fprintf(stderr, "error: incorrect stdin format\n");
        return 1;
      }
      elf.callories += converted;
    }
  }

  if (elf.id < num_top_elves) {
    fprintf(stderr, "error: lack of input for listing the top %d elves\n",
            num_top_elves);
    return 1;
  }

  print_top_elves(top, num_top_elves);

  return 0;
}
