/**
 * @title Advent of Code - 16
 * @author Mifka Radim
 **/

#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define REALLOC_AMOUNT 10

typedef struct neighbors_arr {
  int size;
  int max_size;
  char **names;
} neighbors_arr;

typedef struct valve {
  char name[3];
  int rate;
  neighbors_arr *neighbors;
} valve;

void add_neighbor(neighbors_arr *arr, char *str) {
  if (arr->size == arr->max_size) {
    arr->names =
        realloc(arr->names, (arr->size + REALLOC_AMOUNT) * sizeof(char *));
    arr->max_size += REALLOC_AMOUNT;
  }

  arr->names[arr->size] = malloc(strlen(str) + 1 * sizeof(char));
  strcpy(arr->names[arr->size], str);
  arr->size++;
}

void init_neighbors_array(neighbors_arr *arr, int length) {
  arr->names = malloc(sizeof(char *) * length);
  arr->size = 0;
  arr->max_size = length;
}

void free_neighbors_array(neighbors_arr *arr) {
  for (int i = 0; i < arr->size; i++) {
    free(arr->names[i]);
  }
  free(arr->names);
  free(arr);
}

unsigned int hash(const char *str) {
  unsigned int hash = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    hash = 31 * hash + str[i];
  }
  return hash;
}

void free_valve(valve *v) {
  if (v) {
    free_neighbors_array(v->neighbors);
    free(v);
  }
}

void print_valve(valve *v) {
  printf("[%s:%d: ", v->name, v->rate);
  for (int i = 0; i < v->neighbors->size; i++) {
    printf("%s ", v->neighbors->names[i]);
  }
  printf("] -> ");
}

void print_map(map *m) {
  for (int i = 0; i < m->max_size; i++) {
    printf("Bucket %d: ", i);

    bucket *current = m->buckets[i];
    while (current != NULL) {
      print_valve(current->value);
      current = current->next;
    }

    printf("NULL\n");
  }
}

int main() {
  char row[MAX_LENGTH + 2];

  map valves;
  init_map(&valves, &hash, NULL, (void *)&free_valve);

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;

    char neighbors[MAX_LENGTH];
    char name[3];
    int rate;

    if (sscanf(row, "Valve %2s has flow rate=%d; tunnels lead to valves %[^\n]",
               name, &rate, neighbors) != 3) {
      if (sscanf(row,
                 "Valve %2s has flow rate=%d; tunnel leads to valve %[^\n]",
                 name, &rate, neighbors) != 3) {
        return 1;
      }
    }

    valve *current_valve = (valve *)malloc(sizeof(valve));
    current_valve->neighbors = (neighbors_arr *)malloc(sizeof(neighbors_arr));
    current_valve->rate = rate;
    init_neighbors_array(current_valve->neighbors, REALLOC_AMOUNT);

    char *token = strtok(neighbors, ", ");
    while (token != NULL) {
      add_neighbor(current_valve->neighbors, token);
      token = strtok(NULL, ", ");
    }

    strncpy(current_valve->name, name, 3);

    add(&valves, current_valve->name, current_valve);
  }
  print_map(&valves);
  free_map(&valves);

  return 0;
}
