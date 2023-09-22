/**
 * @title Advent of Code - 17
 * @author Mifka Radim
 **/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROCK_LENGTH 4
#define REALLOC_AMOUNT 100
#define MAX_ROCKS 2023

#define CHAMBER_WIDTH 7
#define CHAMBER_HEIGHT 3
#define SPAWN_SPACE 3

#define AIR 0
#define ROCK 1
#define SOLID 2

#define MOVE_RIGHT '>'
#define MOVE_LEFT '<'
#define MOVE_DOWN 'd'

typedef struct point {
  int x;
  int y;
} point;

typedef struct array_list {
  int size;
  int max_size;

  int *maxx;
  int *minx;
  int *miny;

  point *items;
} array_list;

array_list rock;
int height = 0;

void add_item(array_list *arr, point p) {
  if (arr->size == arr->max_size) {
    arr->items =
        realloc(arr->items, (arr->size + REALLOC_AMOUNT) * sizeof(point));
    arr->max_size += REALLOC_AMOUNT;
  }

  if (arr->maxx == NULL || p.x > *arr->maxx)
    arr->maxx = &arr->items[arr->size].x;

  if (arr->minx == NULL || p.x < *arr->minx)
    arr->minx = &arr->items[arr->size].x;

  if (arr->miny == NULL || p.y < *arr->miny)
    arr->miny = &arr->items[arr->size].y;

  arr->items[arr->size] = p;
  arr->size++;
}

void init_array_list(array_list *arr, int length) {
  arr->items = malloc(sizeof(point) * length);

  arr->maxx = NULL;
  arr->miny = NULL;
  arr->minx = NULL;

  arr->size = 0;
  arr->max_size = length;
}

void free_array_list(array_list *arr) { free(arr->items); }

void fill_matrix(int ***matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      (*matrix)[i][j] = AIR;
    }
  }
}

void print_matrix(int **matrix, int rows, int cols) {
  for (int i = rows - 1; i >= 0; i--) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == AIR)
        printf(".");
      else if (matrix[i][j] == ROCK)
        printf("@");
      else if (matrix[i][j] == SOLID)
        printf("#");
    }
    printf("\n");
  }
}

int add_rows(int ***matrix, int *rows, int cols, int number) {

  if (number <= 0) {
    for (int i = *rows + number; i < *rows; i++) {
      free((*matrix)[i]);
    }
    *rows += number;
    return 0;
  }

  *matrix = (int **)realloc(*matrix, ((*rows) + number) * sizeof(int *));

  if (*matrix == NULL) {
    fprintf(stderr, "Memory reallocation failed\n");
    return 1;
  }

  for (int i = *rows; i < (*rows) + number; i++) {
    (*matrix)[i] = malloc(cols * sizeof(int));
    if ((*matrix)[i] == NULL) {
      fprintf(stderr, "Memory allocation failed for row %d of new matrix\n", i);
      return 1;
    }
    for (int j = 0; j < cols; j++) {
      (*matrix)[i][j] = AIR;
    }
  }
  *rows += number;
  return 0;
}

void save_rock(int ***matrix, int type) {
  for (int i = 0; i < rock.size; i++) {
    int x = rock.items[i].x;
    int y = rock.items[i].y;

    if (type == SOLID && y + 1 > height)
      height = y + 1;

    (*matrix)[y][x] = type;
  }
}

void clear_rock() {
  rock.miny = NULL;
  rock.minx = NULL;
  rock.maxx = NULL;
  rock.size = 0;
}

int next_solid(int ***matrix, int direction) {

  for (int i = 0; i < rock.size; i++) {
    point p = rock.items[i];

    switch (direction) {
    case MOVE_DOWN:
      if ((*matrix)[p.y - 1][p.x] == SOLID)
        return 1;
      break;
    case MOVE_RIGHT:
      if ((*matrix)[p.y][p.x + 1] == SOLID)
        return 1;
      break;
    case MOVE_LEFT:
      if ((*matrix)[p.y][p.x - 1] == SOLID)
        return 1;
      break;
    }
  }
  return 0;
}

int move_rock(int ***matrix, int direction) {

  switch (direction) {
  case MOVE_RIGHT:
    if (*rock.maxx == CHAMBER_WIDTH - 1 || next_solid(matrix, direction))
      return 0;
    break;
  case MOVE_LEFT:
    if (*rock.minx == 0 || next_solid(matrix, direction))
      return 0;
    break;
  case MOVE_DOWN:
    if (*rock.miny == 0 || next_solid(matrix, direction)) {
      save_rock(matrix, SOLID);
      return 1;
    }
    break;
  }

  for (int i = 0; i < rock.size; i++) {

    int *x = &rock.items[i].x;
    int *y = &rock.items[i].y;

    (*matrix)[*y][*x] = AIR;

    if (direction == '>' && *x < CHAMBER_WIDTH)
      (*x)++;

    if (direction == '<' && *x > 0)
      (*x)--;

    if (direction == 'd' && *y > 0)
      (*y)--;
  }

  save_rock(matrix, ROCK);

  return 0;
}

void spawn_rock(int ***matrix, int *rows, int cols, int rocks) {
  (void)cols;
  clear_rock();
  point p;

  int free_space = *rows - height - SPAWN_SPACE;

  switch (rocks) {
  case 0:
    add_rows(matrix, rows, cols, 1 - free_space);
    p.y = *rows - 1;
    for (int i = 5; i > 1; i--) {
      p.x = i;
      add_item(&rock, p);
    }
    break;
  case 1:
    add_rows(matrix, rows, cols, 3 - free_space);
    p.x = 3;
    p.y = *rows - 1;
    add_item(&rock, p);
    p.y -= 2;
    add_item(&rock, p);
    p.y++;
    for (int i = 2; i < 5; i++) {
      p.x = i;
      add_item(&rock, p);
    }
    break;
  case 2:
    add_rows(matrix, rows, cols, 3 - free_space);
    p.x = 4;
    p.y = *rows - 1;
    add_item(&rock, p);
    p.y--;
    add_item(&rock, p);
    p.y--;
    for (int i = 2; i < 5; i++) {
      p.x = i;
      add_item(&rock, p);
    }
    break;
  case 3:
    add_rows(matrix, rows, cols, 4 - free_space);
    p.x = 2;
    for (int i = 1; i < 5; i++) {
      p.y = *rows - i;
      add_item(&rock, p);
    }
    break;
  case 4:
    add_rows(matrix, rows, cols, 2 - free_space);
    p.x = 2;
    p.y = *rows - 1;
    add_item(&rock, p);
    p.y--;
    add_item(&rock, p);

    p.x++;

    p.y++;
    add_item(&rock, p);
    p.y--;
    add_item(&rock, p);
    break;
  }
  save_rock(matrix, ROCK);
}

void free_matrix(int ***matrix, int rows, int cols) {
  (void)cols;
  (void)rows;

  for (int i = 0; i < rows; i++) {
    free((*matrix)[i]);
  }
  free(*matrix);
}

int main() {

  init_array_list(&rock, REALLOC_AMOUNT);

  int rocks = 0;
  int direction;

  int rows = CHAMBER_HEIGHT;
  int cols = CHAMBER_WIDTH;

  int **matrix = malloc(rows * sizeof(int *));

  for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
  }

  fill_matrix(&matrix, rows, cols);

  int solid = 1;

  while (1) {

    if (rocks == MAX_ROCKS)
      break;

    direction = getchar();

    if (direction == '\n') {
      fseek(stdin, 0, SEEK_SET);
    } else {
      if (solid) {
        spawn_rock(&matrix, &rows, cols, rocks % (ROCK_LENGTH + 1));
        solid = 0;
        rocks++;
      }
      move_rock(&matrix, direction);
      solid = move_rock(&matrix, MOVE_DOWN);
    }
  }

  printf("Tower of rocks is %d units tall.\n", height);

  free_matrix(&matrix, rows, cols);
  free_array_list(&rock);
  return 0;
}
