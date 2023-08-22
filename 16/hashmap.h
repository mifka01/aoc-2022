#ifndef HASHMAP_H_
#define HASHMAP_H_

#define MAX_LOAD_FACTOR 0.7
#define HASHMAP_REALLOC_AMOUNT 20

typedef struct bucket {
  char *key;
  void *value;
  struct bucket *next;
} bucket;

typedef struct map {
  bucket **buckets;
  int size;
  int max_size;
  unsigned int (*hash)(const char *key);
} map;

void init_map(map *m, unsigned int (*hash)(const char *key));

void free_map(map *m);

int rehash(map *m, int new_size);

int add(map *m, char *key, void *value);

void *get(map *m, char *key);

#endif
