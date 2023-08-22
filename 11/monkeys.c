/**
 * @title Advent of Code - 11
 * @author Mifka Radim
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define REALLOC_AMOUNT 5
#define MONKEYS 10
#define MAX_ITEMS 10
#define ROUNDS 10000

typedef struct monkey {
  int id;
  long long int *items;
  int items_len;
  int items_max;
  char operation[5];
  int divisible;
  int t;
  int f;
  int inspected;

} monkey;

typedef struct monkey_arr {
  int size;
  int max_size;
  monkey *monkeys;

} monkey_arr;

void print_monkeys(monkey_arr arr) {
  for (int i = 0; i < arr.size; i++) {
    printf("Monkey %d:", arr.monkeys[i].id);
    for (int j = 0; j < arr.monkeys[i].items_len; j++) {
      printf(" %lld", arr.monkeys[i].items[j]);
    }
    printf(", inspects: %d", arr.monkeys[i].inspected);
    printf("\n");
  }
}

void init_monkey_array(monkey_arr *arr, int length) {
  arr->monkeys = malloc(sizeof(monkey) * length);
  arr->size = 0;
  arr->max_size = length;
}

int add_monkey(monkey_arr *arr, monkey new_monkey) {

  if (arr->size == arr->max_size) {
    arr->max_size += REALLOC_AMOUNT;

    monkey *ptr = realloc(arr->monkeys, arr->max_size * sizeof(monkey));

    if (ptr == NULL) {
      printf("Error: Memory allocation failed during monkey_arr resize.\n");
      return 1;
    }
    arr->monkeys = ptr;
  }

  arr->monkeys[arr->size] = new_monkey;
  arr->size++;
  return 0;
}

int add_monkey_item(monkey *monkey, long long int item) {

  if (monkey->items_len == monkey->items_max) {
    monkey->items_max += REALLOC_AMOUNT;
    long long int *ptr =
        realloc(monkey->items, monkey->items_max * sizeof(long long int));
    if (ptr == NULL) {
      printf("Error: Memory allocation failed during monkey_arr resize.\n");
      return 1;
    }
    monkey->items = ptr;
  }
  monkey->items[monkey->items_len] = item;
  monkey->items_len++;
  return 0;
}

void init_monkey(monkey *monkey, int max_items) {
  monkey->items_len = 0;
  monkey->items_max = max_items;
  monkey->inspected = 0;
  monkey->items = malloc(max_items * sizeof(long long int));
}

void free_monkeys(monkey_arr *arr) {
  for (int i = 0; i < arr->size; i++) {
    free(arr->monkeys[i].items);
  }
  free(arr->monkeys);
}

int parse_input_items(char *row, monkey *monkey) {
  int item;
  char *pEnd;
  item = strtol(row, &pEnd, 10);
  while (strlen(pEnd) != 0) {
    if (add_monkey_item(monkey, item))
      return 1;
    item = strtol(pEnd + 2, &pEnd, 10);
  }
  if (add_monkey_item(monkey, item))
    return 1;

  return 0;
}

int get_divisitor(monkey_arr monkeys) {
  int div = monkeys.monkeys[0].divisible;

  for (int i = 1; i < monkeys.size; i++) {
    div *= monkeys.monkeys[i].divisible;
  }
  return div;
}

void inspect_item(monkey *monkey, monkey_arr *monkeys) {
  long long int number;
  char operator;
  for (int i = 0; i < monkey->items_len; i++) {
    operator= monkey->operation[0];
    if (strncmp(monkey->operation + 2, "old", 3) == 0)
      number = monkey->items[i];
    else
      number = atoi(monkey->operation + 2);

    switch (operator) {
    case '+':
      number += monkey->items[i];
      break;
    case '*':
      number *= monkey->items[i];
      break;
    }
    number %= get_divisitor(*monkeys);

    if (number % monkey->divisible == 0) {
      add_monkey_item(&monkeys->monkeys[monkey->t], number);
    } else {
      add_monkey_item(&monkeys->monkeys[monkey->f], number);
    }
    monkey->inspected++;
  }
  monkey->items_len = 0;
}

long long int get_business_level(monkey_arr arr) {
  long long int largest = 0;
  long long int second_largest = -1;

  for (int i = 0; i < arr.size; i++) {
    if (arr.monkeys[i].inspected > largest) {
      second_largest = largest;
      largest = arr.monkeys[i].inspected;
    } else if (arr.monkeys[i].inspected > second_largest &&
               arr.monkeys[i].inspected != largest) {
      second_largest = arr.monkeys[i].inspected;
    }
  }
  return largest * second_largest;
}

int main() {
  char row[MAX_LENGTH + 2];
  int monkey_rows = 0;
  monkey_arr monkey_arr;
  init_monkey_array(&monkey_arr, MONKEYS);
  monkey monkey;
  init_monkey(&monkey, MAX_ITEMS);

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) < 2)
      continue;

    if (monkey_rows == 6)
      monkey_rows = 0;

    switch (monkey_rows) {
    case 0:
      monkey.id = (int)strtol(row + strlen("Monkey "), NULL, 10);
      break;
    case 1:
      if (parse_input_items(row + strlen("  Starting items: "), &monkey)) {
        free(monkey.items);
        free_monkeys(&monkey_arr);
        return 1;
      }
      break;
    case 2:
      strncpy(monkey.operation, row + strlen("  Operation: new = old "), 5);
      break;
    case 3:
      monkey.divisible =
          (int)strtol(row + strlen("  Test: divisible by "), NULL, 10);
      break;
    case 4:
      monkey.t =
          (int)strtol(row + strlen("    If true: throw to monkey "), NULL, 10);
      break;
    case 5:
      monkey.f =
          (int)strtol(row + strlen("    If false: throw to monkey "), NULL, 10);
      break;
    }

    if (monkey_rows == 5) {
      if (add_monkey(&monkey_arr, monkey)) {
        free(monkey.items);
        free_monkeys(&monkey_arr);
        return 1;
      }
      init_monkey(&monkey, MAX_ITEMS);
    }
    monkey_rows++;
  }
  for (int i = 0; i < ROUNDS; i++) {
    for (int j = 0; j < monkey_arr.size; j++) {
      inspect_item(&monkey_arr.monkeys[j], &monkey_arr);
    }
  }
  print_monkeys(monkey_arr);

  printf("The level of monkey business: %lld\n",
         get_business_level(monkey_arr));
  free(monkey.items);
  free_monkeys(&monkey_arr);

  return 0;
}
