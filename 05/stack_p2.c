
/**
 * @title Advent of Code - 05
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

int count_spaces(char *str) {
  int count = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == ' ')
      count++;
  }
  return count;
}

typedef struct {
  int top;
  char items[MAX_LENGTH];
} t_stack;

void init_stack(t_stack *stack) { stack->top = -1; }

char pop_stack(t_stack *stack) {
  if (stack->top < 0)
    return -1;

  char popped = stack->items[stack->top];
  stack->items[stack->top] = '\0';
  stack->top--;
  return popped;
}

int push_stack(t_stack *stack, char val) {
  if (stack->top + 1 >= MAX_LENGTH)
    return 1;

  stack->top++;
  stack->items[stack->top] = val;
  return 0;
}

void print_stack(t_stack stack) {
  for (int i = stack.top; i >= 0; i--) {
    printf("[%c]\n", stack.items[i]);
  }
}

void reverse_stack(t_stack *stack) {
  t_stack tmp_stack;
  init_stack(&tmp_stack);

  while (stack->top != -1) {
    push_stack(&tmp_stack, pop_stack(stack));
  }

  *stack = tmp_stack;
}

void reverse_stacks(t_stack *stacks, int numo_stacks) {
  for (int i = 0; i < numo_stacks; i++) {
    reverse_stack(&stacks[i]);
  }
}

int max_stack(t_stack *stacks, int numo_stacks) {
  int max = 0;
  for (int i = 0; i < numo_stacks; i++) {
    if (stacks[i].top > max)
      max = stacks[i].top;
  }
  return max;
}

void print_stacks(t_stack *stacks, int numo_stacks) {
  int max_top = max_stack(stacks, numo_stacks);

  for (int i = max_top; i >= 0; i--) {
    for (int j = 0; j < numo_stacks; j++) {
      if (i > stacks[j].top)
        printf("    ");
      else
        printf("[%c] ", stacks[j].items[i]);
    }
    printf("\n");
  }

  for (int i = 0; i < numo_stacks; i++) {
    printf(" %d  ", i + 1);
    if (i + 1 == numo_stacks)
      printf("\n");
  }
}

int main() {

  char row[MAX_LENGTH + 2];
  int numo_stacks = 0;
  int loaded = 0;
  int reversed = 0;
  t_stack *stacks;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    if (!numo_stacks) {
      numo_stacks = strlen(row) / 4;
      stacks = malloc(sizeof(t_stack) * numo_stacks);
      if (stacks == NULL)
        return 1;

      for (int i = 0; i <= numo_stacks; i++) {
        init_stack(&stacks[i]);
      }
    }

    if (!loaded) {
      loaded = (row[0] == '\n');
      int i = 0;

      while (i < (int)strlen(row)) {

        if (row[i + 1] != ' ' && !isdigit(row[i + 1]) && !loaded)
          push_stack(&stacks[i / 4], row[i + 1]);
        i += 4;
      }

    } else {
      // stdin is reversed so we have to reverse them
      if (!reversed) {
        reverse_stacks(stacks, numo_stacks);
        reversed = 1;
      }

      int count, from, to;

      if (sscanf(row, "move %d from %d to %d", &count, &from, &to) == 0) {
        return 1;
      }

      t_stack tmp_stack;
      init_stack(&tmp_stack);

      while (count != 0) {
        push_stack(&tmp_stack, pop_stack(&stacks[from - 1]));
        count--;
      }

      while (tmp_stack.top != -1) {
        int el = pop_stack(&tmp_stack);
        push_stack(&stacks[to - 1], el);
      }
    }
  }

  print_stacks(stacks, numo_stacks);
  free(stacks);
  return 0;
}
