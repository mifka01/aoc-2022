/**
 * @title Advent of Code - 16
 * @author Mifka Radim
 **/

#include "hashmap.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define MINUTES 30
#define REALLOC_AMOUNT 100
#define NAME_LENGTH 3

map valves;
map distances;

typedef struct neighbor {
  char name[NAME_LENGTH];
  int distance;
} neighbor;

typedef struct array_list {
  int size;
  int max_size;
  neighbor *items;
} array_list;

typedef struct valve {
  char name[NAME_LENGTH];
  int rate;
  int history;
  array_list neighbors;
} valve;

void init_neighbor(neighbor *n, char *str, int distance) {
  strncpy(n->name, str, NAME_LENGTH);
  n->distance = distance;
}

void add_item(array_list *arr, neighbor nb) {
  if (arr->size == arr->max_size) {
    arr->items =
        realloc(arr->items, (arr->size + REALLOC_AMOUNT) * sizeof(neighbor *));
    arr->max_size += REALLOC_AMOUNT;
  }

  arr->items[arr->size] = nb;
  arr->size++;
}

void init_array_list(array_list *arr, int length) {
  arr->items = malloc(sizeof(neighbor *) * length);
  arr->size = 0;
  arr->max_size = length;
}

void init_valve(valve *v, char *str, int rate) {
  strncpy(v->name, str, NAME_LENGTH);
  v->rate = rate;
  v->history = 0;
  init_array_list(&v->neighbors, REALLOC_AMOUNT);
}

void free_array_list(array_list *arr) { free(arr->items); }

unsigned int hash(const char *str) {
  unsigned int hash = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    hash = 31 * hash + str[i];
  }
  return hash;
}

void free_valve(valve *v) {
  if (v) {
    free_array_list(&v->neighbors);
    free(v);
  }
}

void print_valve(valve *v) {
  printf("%d: ", v->rate);
  for (int i = 0; i < v->neighbors.size; i++) {
    printf("%sd%d ", v->neighbors.items[i].name,
           v->neighbors.items[i].distance);
  }
  printf("] -> ");
}

void print_int(int *d) { printf("%d] -> ", *d); }

void print_map(map *m, void (*print_value)(void *)) {
  for (int i = 0; i < m->max_size; i++) {
    printf("Bucket %d: ", i);

    bucket *current = m->buckets[i];
    while (current != NULL) {
      printf("[%s:", current->key);
      print_value(current->value);
      current = current->next;
    }

    printf("NULL\n");
  }
}

void reset_distances() {

  for (int i = 0; i < distances.max_size; i++) {
    bucket *current_node = distances.buckets[i];
    while (current_node != NULL) {
      int *dist = current_node->value;
      *dist = INT_MAX;
      current_node = current_node->next;
    }
  }
}

void get_distance(map *m, char *current_str, int distance) {

  valve *current = get(m, current_str);

  if (current->history)
    return;

  int *dist = get(&distances, current_str);
  if (*dist > distance)
    *dist = distance;

  current->history = 1;

  for (int i = 0; i < current->neighbors.size; i++) {
    get_distance(m, current->neighbors.items[i].name, distance + 1);
  }

  current->history = 0;
}

int dfs(map *graph, valve *start, int pressure, int minutes) {

  if (start->history)
    return 0;

  if (minutes <= 0)
    return 0;

  start->history = 1;

  int maxPressure = pressure;

  for (int i = 0; i < start->neighbors.size; i++) {
    valve *neighbor = get(graph, start->neighbors.items[i].name);
    int distance = start->neighbors.items[i].distance;
    int subPathPressure = dfs(
        graph, neighbor, pressure + neighbor->rate * (minutes - (distance + 1)),
        minutes - (distance + 1));
    maxPressure =
        (subPathPressure > maxPressure) ? subPathPressure : maxPressure;
  }

  start->history = 0;

  return maxPressure;
}

int main() {
  char row[MAX_LENGTH + 2];
  map graph;

  init_map(&valves, &hash, NULL, (void *)&free_valve);
  init_map(&graph, &hash, NULL, (void *)&free_valve);
  init_map(&distances, &hash, NULL, free);

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
    init_valve(current_valve, name, rate);
    int base_distance = INT_MAX;

    char *token = strtok(neighbors, ", ");
    while (token != NULL) {
      neighbor nb;
      init_neighbor(&nb, token, 1);
      add_item(&current_valve->neighbors, nb);
      token = strtok(NULL, ", ");
    }

    add(&valves, current_valve->name, current_valve, sizeof(valve));
    add(&distances, current_valve->name, &base_distance, sizeof(int));

    if (rate > 0 || strcmp(name, "AA") == 0) {
      init_array_list(&current_valve->neighbors, REALLOC_AMOUNT);
      add(&graph, current_valve->name, current_valve, sizeof(valve));
    }
    free(current_valve);
  }

  for (int i = 0; i < graph.max_size; i++) {
    bucket *current_node = graph.buckets[i];
    while (current_node != NULL) {
      valve *source_valve = get(&graph, current_node->key);
      reset_distances();
      get_distance(&valves, source_valve->name, 0);

      // Loop through neighbors
      for (int j = 0; j < graph.max_size; j++) {
        bucket *current_neighbor = graph.buckets[j];
        while (current_neighbor != NULL) {
          if (strcmp(current_node->key, current_neighbor->key) != 0) {
            // Create a new neighbor and add it to the source valve
            neighbor new_neighbor;
            int *dist = get(&distances, current_neighbor->key);
            init_neighbor(&new_neighbor, current_neighbor->key, *dist);
            add_item(&source_valve->neighbors, new_neighbor);
          }
          current_neighbor = current_neighbor->next;
        }
      }
      current_node = current_node->next;
    }
  }

  valve *start = get(&graph, "AA");
  int maximum = dfs(&graph, start, 0, 30);

  printf("Pressure released: %d\n", maximum);

  free_map(&valves);
  free_map(&graph);
  free_map(&distances);

  return 0;
}
