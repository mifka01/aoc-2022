#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_map(map *m, unsigned int (*hash)(const char *key)) {
  m->size = 0;
  m->max_size = HASHMAP_REALLOC_AMOUNT;
  m->buckets = malloc(HASHMAP_REALLOC_AMOUNT * sizeof(bucket *));
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

int add(map *m, char *key, void *value) {

  if ((float)m->size / m->max_size > MAX_LOAD_FACTOR) {
    rehash(m, m->max_size + HASHMAP_REALLOC_AMOUNT);
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

void *get(map *m, char *key) {
  bucket **bucket = &m->buckets[m->hash(key) % m->max_size];
  while (*bucket) {
    if (strcmp(key, (*bucket)->key) == 0)
      break;
    bucket = &(*bucket)->next;
  }
  return (*bucket)->value;
}
