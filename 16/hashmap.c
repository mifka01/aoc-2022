#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_map(map *m, unsigned int (*hash)(const char *key),
              void (*free_key)(void *), void (*free_value)(void *)) {
  m->size = 0;
  m->max_size = HASHMAP_REALLOC_AMOUNT;
  m->buckets = calloc(HASHMAP_REALLOC_AMOUNT, sizeof(bucket *));
  m->hash = hash;
  m->free_key = free_key;
  m->free_value = free_value;
}

void free_bucket(bucket *b, void (*free_key)(void *),
                 void (*free_value)(void *)) {
  if (b) {
    if (free_key != NULL)
      free_key(b->key);
    else
      free(b->key);

    if (free_value != NULL)
      free_value(b->value);
    else
      free(b->value);

    free_bucket(b->next, free_key, free_value);
    free(b);
  }
}

void free_map(map *m) {

  for (int i = 0; i < m->max_size; i++) {
    free_bucket(m->buckets[i], m->free_key, m->free_value);
  }
  free(m->buckets);
}

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

int add(map *m, char *key, void *value, size_t value_size) {

  if ((float)m->size / m->max_size > MAX_LOAD_FACTOR) {
    rehash(m, m->max_size + HASHMAP_REALLOC_AMOUNT);
  }

  bucket *node = NULL; // new item we want to add
  bucket **bucket =
      &m->buckets[m->hash(key) %
                  m->max_size]; // bucket at index where we wanna add
  while (*bucket) {
    if (strcmp(key, (*bucket)->key) == 0)
      break;
    bucket = &(*bucket)->next;
  }

  // already in hashtable - replace
  if (*bucket) {
    if (m->free_key != NULL)
      m->free_key((*bucket)->key);
    if (m->free_value != NULL)
      m->free_value((*bucket)->value);
    node = *bucket;
  }
  // new - insert
  else {
    node = malloc(sizeof(*node));
    if (node == NULL) {
      printf("Error: Memory allocation failed during adding bucket.\n");
      return 1;
    }
    node->next = NULL;
    *bucket = node;
  }

  node->key = malloc((strlen(key) + 1) * sizeof(char));
  if (node->key == NULL) {
    free(node);
    printf("Error: Memory allocation failed for key.\n");
    return 1;
  }
  strcpy(node->key, key);

  node->value = malloc(value_size);
  if (node->value == NULL) {
    free(node->key);
    free(node);
    printf("Error: Memory allocation failed for value.\n");
    return 1;
  }
  memcpy(node->value, value, value_size);

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
