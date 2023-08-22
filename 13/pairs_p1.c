/**
 * @title Advent of Code - 13
 * @author Mifka Radim
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256
#define PAIRS 150

typedef struct list {
  int is_integer;
  int value;
  int size;
  struct list *list;
  struct list *next;
} list;

typedef struct pair {
  list *left;
  list *right;
} pair;

void print_list(list *root) {
  if (root == NULL) {
    return;
  }

  if (root->is_integer) {
    printf("%d ", root->value);
  } else {
    printf("[ ");
    print_list(root->list);
    printf("] ");
  }

  print_list(root->next);
}

void print_pairs(pair *pairs, int size) {
  for (int i = 0; i <= size; i++) {
    printf("left ");
    print_list(pairs[i].left);
    printf("right ");
    print_list(pairs[i].right);
    printf("\n\n");
  }
}

list *init_list() {
  list *l = malloc(sizeof(list));
  l->is_integer = 0;
  l->size = 0;
  l->list = NULL;
  l->next = NULL;
  return l;
}

void add_integer(list *l, int value) {
  l->size++;

  if (l->list == NULL) {
    l->list = init_list();
    l->list->is_integer = 1;
    l->list->value = value;
    return;
  }

  list *last = l->list;
  while (last->next != NULL)
    last = last->next;

  last->next = init_list();
  last->next->is_integer = 1;
  last->next->value = value;
}

int closing_bracket(char *str) {
  int opened = 1;
  int index = 1;
  while (str[index] != '\0') {
    if (str[index] == '[')
      opened++;
    if (str[index] == ']')
      opened--;
    if (opened == 0)
      break;
    index++;
  }
  return index + 1;
}

void fill_list(list *l, char *str) {
  for (int i = 1; str[i] != '\0'; i++) {
    if (str[i] == ',')
      continue;
    else if (str[i] == '[') {
      int index = closing_bracket(str + i);
      char *part = malloc(sizeof(char) * (index + 1));
      strncpy(part, str + i, index);
      part[index] = '\0';

      if (l->list == NULL) {
        l->list = init_list();
        fill_list(l->list, part);
      } else {
        list *last = l->list;
        while (last->next != NULL)
          last = last->next;

        last->next = init_list();
        fill_list(last->next, part);
      }

      free(part);
      i += index;
      l->size++;

    } else if (str[i] != ']') {
      add_integer(l, atoi(str + i));
    }
  }
}

void free_list(list *l) {
  list *tmp = NULL;

  while (l != NULL) {
    tmp = l;
    l = l->next;
    if (tmp->list != NULL)
      free_list(tmp->list);
    free(tmp);
  }
}

void free_pairs(pair **pairs, int size) {

  for (int i = 0; i <= size; i++) {
    free_list((*pairs)[i].left);
    free_list((*pairs)[i].right);
  }
  free(*pairs);
}

int compare_lists(list *left, list *right) {

  if (left == NULL && right == NULL) {
    return 0;
  }

  if (left == NULL && right != NULL) {
    return 1;
  }

  if (left != NULL && right == NULL) {
    return -1;
  }

  if (left->is_integer && right->is_integer) {

    if (left->value > right->value)
      return -1;

    if (left->value < right->value) {
      return 1;
    }

    int sub = compare_lists(left->next, right->next);
    if (sub != 0)
      return sub;
  }

  if (!left->is_integer && !right->is_integer) {

    int sub = compare_lists(left->list, right->list);
    if (sub != 0)
      return sub;

    sub = compare_lists(left->next, right->next);
    if (sub != 0)
      return sub;
  }

  if (!left->is_integer && right->is_integer) {
    int tmp = right->value;
    list *tmp_n = right->next;
    right = init_list();
    right->next = tmp_n;
    add_integer(right, tmp);
    int sub = compare_lists(left, right);
    free(right->list);
    free(right);
    if (sub != 0)
      return sub;
  }

  if (!right->is_integer && left->is_integer) {
    int tmp = left->value;
    list *tmp_n = right->next;
    left = init_list();
    left->next = tmp_n;
    add_integer(left, tmp);
    int sub = compare_lists(left, right);
    free(left->list);
    free(left);
    if (sub != 0)
      return sub;
  }

  return 0;
}

int main() {
  char row[MAX_LENGTH + 2];
  pair *pairs = malloc(sizeof(pair) * PAIRS);
  int pairs_size = 0;
  int left = 1;
  list *cur_list;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (row[0] == '\0') {
      pairs_size++;
      continue;
    }

    cur_list = init_list();

    fill_list(cur_list, row);

    if (left)
      pairs[pairs_size].left = cur_list;
    else
      pairs[pairs_size].right = cur_list;

    left = !left;
  }

  int indices_sum = 0;
  for (int i = 0; i <= pairs_size; i++) {
    if (compare_lists(pairs[i].left, pairs[i].right) == 1)
      indices_sum += i + 1;
  }
  printf("Sum of indices: %d\n", indices_sum);
  free_pairs(&pairs, pairs_size);
  return 0;
}
