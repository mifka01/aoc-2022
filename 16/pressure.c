/**
 * @title Advent of Code - 16
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define REALLOC_AMOUNT 10
#define MAX_LOAD_FACTOR 0.7

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

  arr->names[arr->size] = str;
  arr->size++;
}

void init_neighbors_array(neighbors_arr *arr, int length) {
  arr->names = malloc(sizeof(char *) * length);
  arr->size = 0;
  arr->max_size = length;
}

void free_neighbors_array(neighbors_arr *arr) { free(arr->names); }

typedef struct bucket {
  char *key;
  valve *value;
  struct bucket *next;
} bucket;

typedef struct map {
  bucket **buckets;
  int size;
  int max_size;
  unsigned int (*hash)(const char *key);
} map;

void init_map(map *m, unsigned int (*hash)(const char *key)) {
  m->size = 0;
  m->max_size = REALLOC_AMOUNT;
  m->buckets = malloc(REALLOC_AMOUNT * sizeof(bucket *));
  m->hash = hash;
}

void free_map(map *m) { free(m->buckets); }

int rehash(map *m, int new_size) {
  bucket **buckets = calloc(new_size, sizeof(bucket *));
  if (buckets == NULL) {
    printf("Error: Memory allocation failed during rehashing map.\n");
    return 1;
  }

  for (int i = 0; i < m->max_size; i++) {
    bucket *current = m->buckets[i];
    while (current != NULL) {
      bucket *next = current->next;
      int hash = m->hash(current->key) % new_size;
      current->next = buckets[hash];
      buckets[hash] = current;
      current = next;
    }
  }

  free(m->buckets);
  m->buckets = buckets;
  m->max_size = new_size;
  return 0;
}

int add(map *m, char *key, valve *value) {

  if ((float)m->size / m->max_size > MAX_LOAD_FACTOR) {
    rehash(m, m->max_size + REALLOC_AMOUNT);
  }

  bucket *node; // new item we want to add
  bucket **bucket =
      &m->buckets[m->hash(key) %
                  m->max_size]; // bucket at index where we wanna add
  while (*bucket) {
    if (strcmp(key, (*bucket)->key) == 0)
      break;
    bucket = &(*bucket)->next;
  }

  // new - insert
  if (*bucket) {
    node = *bucket;
  }
  // already in hashtable - replace
  else {
    node = malloc(sizeof(*node));
    if (node == NULL) {
      printf("Error: Memory allocation failed during adding bucket.\n");
      return 1;
    }
    node->next = NULL;
    *bucket = node;
  }

  node->key = key;
  node->value = value;

  m->size++;
  return 0;
}

valve *get(map *m, char *key) {
  bucket **bucket = &m->buckets[m->hash(key) % m->max_size];
  while (*bucket) {
    if (strcmp(key, (*bucket)->key) == 0)
      break;
    bucket = &(*bucket)->next;
  }
  return (*bucket)->value;
}

unsigned int hash(const char *str) {
  unsigned int hash = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    hash = 31 * hash + str[i];
  }
  return hash;
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
  init_map(&valves, &hash);

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
      add_neighbor(current_valve->neighbors, strdup(token));
      token = strtok(NULL, ", ");
    }

    strncpy(current_valve->name, name, 3);

    add(&valves, current_valve->name, current_valve);
  }
  print_map(&valves);

  return 0;
}
