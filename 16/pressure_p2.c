/**
 * @title Advent of Code - 16
 * @author Mifka Radim
 **/

#include "hashmap.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define MINUTES 26
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
  size_t value_size;
  void **items;
  void (*free_value)(void *);
} array_list;

typedef struct valve {
  char name[NAME_LENGTH];
  int rate;
  int history;
  array_list neighbors;
} valve;

typedef struct path {
  array_list nodes;
  int pressure;
} path;

array_list paths;

void init_neighbor(neighbor *n, char *str, int distance) {
  strncpy(n->name, str, NAME_LENGTH);
  n->distance = distance;
}

void add_item(array_list *arr, void *value) {
  if (arr->size == arr->max_size) {
    arr->items =
        realloc(arr->items, (arr->size + REALLOC_AMOUNT) * arr->value_size);
    arr->max_size += REALLOC_AMOUNT;
  }

  arr->items[arr->size] = malloc(arr->value_size);
  memcpy(arr->items[arr->size], value, arr->value_size);
  arr->size++;
}

void init_array_list(array_list *arr, int length, size_t value_size,
                     void (*free_value)(void *)) {
  arr->items = malloc(value_size * length);
  arr->value_size = value_size;
  arr->size = 0;
  arr->max_size = length;
  arr->free_value = free_value;
}

void free_array_list(array_list *arr) {
  for (int i = 0; i < arr->size; i++) {
    if (arr->free_value != NULL)
      arr->free_value(arr->items[i]);
    else
      free(arr->items[i]);
  }
  free(arr->items);
}

void free_path(path *p) {
  free_array_list(&p->nodes);
  free(p);
}

void init_path(path *p, int pressure) {
  init_array_list(&p->nodes, REALLOC_AMOUNT, sizeof(char) * NAME_LENGTH, free);
  p->pressure = pressure;
}

void init_valve(valve *v, char *str, int rate) {
  strncpy(v->name, str, NAME_LENGTH);
  v->rate = rate;
  v->history = 0;
  init_array_list(&v->neighbors, REALLOC_AMOUNT, sizeof(neighbor *), NULL);
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
    free_array_list(&v->neighbors);
    free(v);
  }
}

void print_valve(valve *v) {
  printf("%d: ", v->rate);
  for (int i = 0; i < v->neighbors.size; i++) {
    neighbor *nb = v->neighbors.items[i];
    printf("%sd%d ", nb->name, nb->distance);
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
    neighbor *nb = current->neighbors.items[i];
    get_distance(m, nb->name, distance + 1);
  }

  current->history = 0;
}

void dfs(map *graph, valve *start, int pressure, int minutes,
         path *current_path) {

  if (start->history)
    return;

  if (minutes <= 0)
    return;

  start->history = 1;

  path new_path;
  init_path(&new_path, pressure);

  if (current_path) {
    for (int i = 0; i < current_path->nodes.size; i++) {
      char *node = current_path->nodes.items[i];
      add_item(&new_path.nodes, node);
    }
  }

  add_item(&new_path.nodes, start->name);

  for (int i = 0; i < start->neighbors.size; i++) {
    neighbor *nb = start->neighbors.items[i];
    valve *neighbor = get(graph, nb->name);

    int distance = nb->distance + 1;

    dfs(graph, neighbor, pressure + neighbor->rate * (minutes - distance),
        minutes - distance, &new_path);
  }

  if (new_path.nodes.size >= start->neighbors.size / 2)
    add_item(&paths, &new_path);
  else
    free_array_list(&new_path.nodes);

  start->history = 0;
}

int are_disjoint(array_list arr1, array_list arr2) {
  for (int i = 1; i < arr1.size; i++) {
    for (int j = 1; j < arr2.size; j++) {
      if (strcmp(arr1.items[i], arr2.items[j]) == 0) {
        return 0;
      }
    }
  }
  return 1;
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
      add_item(&current_valve->neighbors, &nb);
      token = strtok(NULL, ", ");
    }

    add(&valves, current_valve->name, current_valve, sizeof(valve));
    add(&distances, current_valve->name, &base_distance, sizeof(int));

    if (rate > 0 || strcmp(name, "AA") == 0) {
      init_array_list(&current_valve->neighbors, REALLOC_AMOUNT,
                      sizeof(neighbor *), NULL);
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
            add_item(&source_valve->neighbors, &new_neighbor);
          }
          current_neighbor = current_neighbor->next;
        }
      }
      current_node = current_node->next;
    }
  }

  init_array_list(&paths, REALLOC_AMOUNT, sizeof(path), (void *)free_path);

  valve *start = get(&graph, "AA");

  int maximum = 0;

  dfs(&graph, start, 0, MINUTES, NULL);

  for (int i = 0; i < paths.size; i++) {
    path *p1 = paths.items[i];
    for (int j = 0; j < paths.size; j++) {
      if (i == j)
        continue;
      path *p2 = paths.items[j];
      if (are_disjoint(p1->nodes, p2->nodes)) {
        if (p1->pressure + p2->pressure > maximum)
          maximum = p1->pressure + p2->pressure;
      }
    }
  }

  printf("Pressure released: %d\n", maximum);

  free_array_list(&paths);

  free_map(&valves);
  free_map(&graph);
  free_map(&distances);

  return 0;
}
