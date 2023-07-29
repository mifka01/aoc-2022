/**
 * @title Advent of Code - 12
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 200
#define REALLOC_AMOUNT 5
#define QUEUE_SIZE 5
#define NEIGHBORS_SIZE 4
// 41
#define ROWS 41
// 113
#define COLS 113

#define FRESH 0
#define OPEN 1
#define CLOSED 2

typedef struct node {
  int x;
  int y;
  int distance;
  char value;
  int status;
  int neighbors_size;
  int neighbors_max;
  struct node **neighbors;
} node;

typedef struct queue {
  int first;
  int last;
  int size;
  node **items;

} queue;

void init_queue(queue *q, int size) {
  q->first = -1;
  q->last = -1;
  q->size = size;
  q->items = (node **)malloc(size * sizeof(node *));
}

int en_queue(queue *queue, node *n) {
  if (queue == NULL) {
    printf("Error: Queue is NULL.\n");
    return 1;
  }
  if (queue->first == -1)
    queue->first = 0;

  if (queue->last == queue->size - 1) {
    queue->size += REALLOC_AMOUNT;
    node **ptr = realloc(queue->items, queue->size * sizeof(node));

    if (ptr == NULL) {
      printf("Error: Memory allocation failed during queue resize.\n");
      return 1;
    }
    queue->items = ptr;
  }

  queue->last++;
  queue->items[queue->last] = n;

  return 0;
}

int empty_queue(queue queue) {
  return !(queue.first > -1 && queue.first <= queue.last);
}

node *de_queue(queue *queue) {

  if (!empty_queue(*queue)) {
    node *item = queue->items[queue->first];
    queue->first++;
    return item;
  }
  return NULL;
}

void clear_queue(queue *queue) {
  if (queue == NULL) {
    return;
  }
  free(queue->items);
}

void print_matrix(node matrix[ROWS][COLS]) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%c", matrix[i][j].value);
    }
    printf("\n");
  }
}

void print_node(node node) {
  printf("Node:\n");
  printf("  value: %d\n", node.value);
  printf("  distance: %d\n", node.distance);
  switch (node.status) {
  case 0:
    printf("  status: FRESH\n");
    break;
  case 1:
    printf("  status: OPEN\n");
    break;
  case 2:
    printf("  status: CLOSED\n");
    break;
  }
  printf("  position: [%d,%d]\n", node.x, node.y);
  printf("  neighbors: ");
  for (int i = 0; i < node.neighbors_size; i++) {
    printf("[%d,%d] ", node.neighbors[i]->x, node.neighbors[i]->y);
  }
  printf("\n");
}

void print_queue(queue queue) {
  for (int i = queue.first; i <= queue.last; i++) {
    print_node(*queue.items[i]);
  }
}

void init_node(node *n, int value, int x, int y, int neighbors_max) {
  n->distance = INT_MAX;
  n->x = x;
  n->y = y;
  n->value = value;
  n->status = FRESH;
  n->neighbors_max = neighbors_max;
  n->neighbors_size = 0;
  n->neighbors = malloc(sizeof(node *) * neighbors_max);
}

int add_neighbor(node *n, node *neighbor) {
  if (n->neighbors_size >= n->neighbors_max) {
    n->neighbors_max += REALLOC_AMOUNT;
    node **ptr = realloc(n->neighbors, n->neighbors_max * sizeof(node *));

    if (ptr == NULL) {
      printf("Error: Memory allocation failed during node neighbors resize.\n");
      return 1;
    }
    n->neighbors = ptr;
  }

  n->neighbors[n->neighbors_size] = neighbor;
  n->neighbors_size++;
  return 0;
}

int is_neighbor(node n1, node n2) {

  if (n1.value >= n2.value - 1)
    return 1;
  return 0;
}

int main() {
  char row[MAX_LENGTH + 2];
  node matrix[ROWS][COLS];
  int row_index = 0;
  node *start;
  node *end;
  node *current;
  queue node_queue;

  init_queue(&node_queue, QUEUE_SIZE);

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) < 2)
      continue;

    for (int col = 0; row[col] != '\0'; col++) {
      init_node(&matrix[row_index][col], row[col], col, row_index,
                NEIGHBORS_SIZE);

      if (row[col] == 'S') {
        matrix[row_index][col].status = OPEN;
        matrix[row_index][col].value = 'a';
        matrix[row_index][col].distance = 0;
        start = &matrix[row_index][col];
      }

      if (row[col] == 'E') {
        matrix[row_index][col].value = 'z';
        end = &matrix[row_index][col];
      }
    }
    row_index++;
  }

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (i < ROWS - 1 && is_neighbor(matrix[i][j], matrix[i + 1][j]))
        add_neighbor(&matrix[i][j], &matrix[i + 1][j]);
      if (i > 0 && is_neighbor(matrix[i][j], matrix[i - 1][j]))
        add_neighbor(&matrix[i][j], &matrix[i - 1][j]);
      if (j < COLS - 1 && is_neighbor(matrix[i][j], matrix[i][j + 1]))
        add_neighbor(&matrix[i][j], &matrix[i][j + 1]);
      if (j > 0 && is_neighbor(matrix[i][j], matrix[i][j - 1]))
        add_neighbor(&matrix[i][j], &matrix[i][j - 1]);
    }
  }

  en_queue(&node_queue, start);

  while (!empty_queue(node_queue)) {
    current = de_queue(&node_queue);
    for (int i = 0; i < current->neighbors_size; i++) {
      if (current->neighbors[i]->status == FRESH) {
        current->neighbors[i]->status = OPEN;
        current->neighbors[i]->distance = current->distance + 1;
        en_queue(&node_queue, current->neighbors[i]);
      }
    }
    current->status = CLOSED;
  }

  print_node(*start);
  print_node(*end);
  return 0;
}
