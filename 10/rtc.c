/**
 * @title Advent of Code - 10
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define REALLOC_AMOUNT 5
#define QUEUE_SIZE 10
#define CRT_WIDTH 40

typedef struct string_queue {
  int first;
  int last;
  int size;
  char **items;

} string_queue;

string_queue create_queue(int size) {
  string_queue q;
  q.first = -1;
  q.last = -1;
  q.size = size;
  q.items = (char **)malloc(sizeof(char *) * size);

  return q;
}

int en_queue(string_queue *queue, char *item) {
  if (queue == NULL) {
    printf("Error: Queue is NULL.\n");
    return 1;
  }
  if (queue->first == -1)
    queue->first = 0;

  if (queue->last == queue->size - 1) {
    queue->size += REALLOC_AMOUNT;
    char **ptr = realloc(queue->items, queue->size * sizeof(char *));

    if (queue->items == NULL) {
      printf("Error: Memory allocation failed during queue resize.\n");
      return 1;
    }
    queue->items = ptr;
  }

  queue->last++;
  queue->items[queue->last] = (char *)malloc(sizeof(char) * (strlen(item) + 1));

  if (queue->items[queue->last] == NULL) {
    printf("Error: Memory allocation failed for new item.\n");
    return 1;
  }
  strcpy(queue->items[queue->last], item);
  return 0;
}

char *de_queue(string_queue *queue) {

  if (queue->first > -1 && queue->first <= queue->last) {
    char *item = queue->items[queue->first];
    queue->first++;
    return item;
  }

  return NULL;
}

void print_queue(string_queue queue) {
  for (int i = 0; i <= queue.last; i++) {
    printf("%d. -> %s\n", i, queue.items[i]);
  }
}

void clear_queue(string_queue *queue) {
  if (queue == NULL) {
    return;
  }
  for (int i = 0; i <= queue->last; i++) {
    free(queue->items[i]);
  }
  free(queue->items);
}

int empty_queue(string_queue queue) { return (queue.first > queue.last); }

void draw_pixel(int draw_position, int sprite_position) {
  if (draw_position == 0)
    draw_position = CRT_WIDTH;

  if (draw_position == sprite_position ||
      draw_position == sprite_position - 1 ||
      draw_position == sprite_position + 1)
    printf("#");
  else
    printf(".");
}

int main() {
  char row[MAX_LENGTH + 2];
  int reg_x = 1;
  int cycle = 1;
  int free = 0;
  int stop = CRT_WIDTH;
  int total = 0;
  string_queue instructions = create_queue(QUEUE_SIZE);
  char *instruction = "noop";

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) < 2)
      continue;

    if (en_queue(&instructions, row)) {
      clear_queue(&instructions);
      return 1;
    }
  }

  while (1) {

    if (free == 0) {
      if (empty_queue(instructions))
        break;

      if (strncmp(instruction, "addx", 4) == 0)
        reg_x += atoi(instruction + 4);

      instruction = de_queue(&instructions);

      if (strcmp(instruction, "noop") == 0)
        free = 1;
      if (strncmp(instruction, "addx", 4) == 0)
        free = 2;
    }

    draw_pixel(cycle % CRT_WIDTH, reg_x + 1);
    if (cycle == stop) {
      total += stop * reg_x;
      stop += CRT_WIDTH;
      printf("\n");
    }

    free--;
    cycle++;
  }

  printf("cycles:%d x: %d total: %d\n", cycle, reg_x, total);
  clear_queue(&instructions);
  return 0;
}
