/**
 * @title Advent of Code - 14
 * @author Mifka Radim
 **/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 512
#define REALLOC_AMOUNT 10
#define PATHS 10
#define PATH_SIZE 10

#define AIR 0
#define ROCK 1
#define SAND 2
#define REST 3
#define TOP 4
#define SKIP 5

#define EXPAND_FLOOR 10

#define POINTS_ARE_EQUAL(pt1, pt2) ((pt1.x == pt2.x) && (pt1.y == pt2.y))

#define IS_ON_X(p, pt1, pt2)                                                   \
  (p.x == pt1.x && p.y >= pt1.y && p.y <= pt2.y) ||                            \
      (p.x == pt1.x && p.y <= pt1.y && p.y >= pt2.y)
#define IS_ON_Y(p, pt1, pt2)                                                   \
  (p.y == pt1.y && p.x >= pt1.x && p.x <= pt2.x) ||                            \
      (p.y == pt1.y && p.x <= pt1.x && p.x >= pt2.x)

typedef struct point {
  int x;
  int y;
} point;

typedef struct solid_path {
  int size;
  int max_size;

  int minx;
  int miny;
  int maxx;
  int maxy;

  point *arr;
} solid_path;

void init_path(solid_path *p, int size) {
  p->arr = malloc(sizeof(point) * size);
  p->size = 0;
  p->max_size = size;
  p->miny = INT_MAX;
  p->minx = INT_MAX;
  p->maxx = INT_MIN;
  p->maxy = INT_MIN;
}

int add_point(solid_path *path, point p) {
  if (path->size == path->max_size) {
    path->max_size += REALLOC_AMOUNT;

    point *ptr = realloc(path->arr, path->max_size * sizeof(point));

    if (ptr == NULL) {
      printf("Error: Memory allocation failed during solid_path points "
             "sresize.\n");
      return 1;
    }
    path->arr = ptr;
  }

  path->arr[path->size] = p;
  path->size++;

  if (p.x < path->minx)
    path->minx = p.x;

  if (p.y < path->miny)
    path->miny = p.y;

  if (p.x > path->maxx)
    path->maxx = p.x;

  if (p.y > path->maxy)
    path->maxy = p.y;

  return 0;
}

void print_matrix(int rows, int cols, int **matrix) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == ROCK)
        printf("#");
      else if (matrix[i][j] == SAND)
        printf("+");
      else if (matrix[i][j] == REST)
        printf("o");
      else
        printf(".");
    }
    printf("\n");
  }
}

void free_matrix(int rows, int ***matrix) {
  for (int i = 0; i < rows; i++) {
    free((*matrix)[i]);
  }
  free(*matrix);
}

int parse_input_row(char *row, solid_path *path) {
  int item;
  int skip = 1;
  char *pEnd;
  item = strtol(row, &pEnd, 10);
  point p;
  p.x = item;
  int x_set = 1;

  while (strlen(pEnd) != 0) {
    item = strtol(pEnd + skip, &pEnd, 10);

    if (x_set) {
      p.y = item;
      add_point(path, p);
    } else {
      p.x = item;
    }

    x_set = !x_set;

    if (skip == 1) {
      skip = 3;
      continue;
    }

    if (skip == 3)
      skip = 1;
  }

  return 0;
}

int is_rock(point p, solid_path *paths, int paths_size) {
  for (int i = 0; i < paths_size; i++) {
    for (int j = 0; j < paths[i].size - 1; j++) {
      if (IS_ON_X(p, paths[i].arr[j], paths[i].arr[j + 1]) ||
          IS_ON_Y(p, paths[i].arr[j], paths[i].arr[j + 1])) {
        return 1;
      }
    }
  }
  return 0;
}

void free_paths(solid_path **paths, int size) {
  for (int i = 0; i < size; i++) {
    free((*paths)[i].arr);
  }
  free(*paths);
}

void expand_floor(int *rows, int *cols, int ***matrix) {
  *cols += EXPAND_FLOOR;
  for (int i = 0; i < *rows; i++) {
    (*matrix)[i] = realloc((*matrix)[i], *cols * sizeof(int));
    for (int j = *cols - EXPAND_FLOOR; j < *cols; j++) {
      (*matrix)[i][j] = AIR;
    }
  }

  for (int i = 0; i < *rows; i++) {
    for (int j = *cols - 1; j >= EXPAND_FLOOR / 2; j--) {
      (*matrix)[i][j] = (*matrix)[i][j - EXPAND_FLOOR / 2];
      (*matrix)[i][j - EXPAND_FLOOR / 2] = AIR;
    }
  }

  for (int j = 0; j < *cols; j++) {
    (*matrix)[*rows - 1][j] = ROCK;
  }
}

int move_sand(point *p, point *start, int *rows, int *cols, int ***matrix) {

  if (p->x == *cols - 1 || p->y == *rows - 1 || p->x <= 0) {
    expand_floor(rows, cols, matrix);
    p->x += EXPAND_FLOOR / 2;
    start->x += EXPAND_FLOOR / 2;
  }

  if ((*matrix)[p->y + 1][p->x] == AIR) {
    (*matrix)[p->y][p->x] = AIR;
    (*matrix)[p->y + 1][p->x] = SAND;
    p->y++;
    return SAND;
  }

  (*matrix)[p->y][p->x] = AIR;
  if (p->x > 0 && (*matrix)[p->y + 1][p->x - 1] != AIR) {
    if ((*matrix)[p->y + 1][p->x + 1] != AIR) {
      (*matrix)[p->y][p->x] = REST;
      return REST;
    }

    p->y++;
    p->x++;
    return move_sand(p, start, rows, cols, matrix);
  }

  p->y++;
  p->x--;
  return move_sand(p, start, rows, cols, matrix);
}

int main() {
  char row[MAX_LENGTH + 2];
  solid_path *paths = malloc(PATHS * sizeof(solid_path));

  int paths_size = 0;
  int max_paths = PATHS;

  int row_num = 0;

  int minx = INT_MAX;
  int maxx = INT_MIN;
  int maxy = INT_MIN;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0) {
      continue;
    }

    if (paths_size == max_paths) {
      max_paths += REALLOC_AMOUNT;
      paths = realloc(paths, (max_paths) * sizeof(solid_path));
    }

    init_path(&paths[row_num], PATH_SIZE);
    parse_input_row(row, &paths[row_num]);
    row_num++;
    paths_size++;
  }

  for (int i = 0; i < paths_size; i++) {
    if (paths[i].minx < minx)
      minx = paths[i].minx;

    if (paths[i].maxx > maxx)
      maxx = paths[i].maxx;

    if (paths[i].maxy > maxy)
      maxy = paths[i].maxy;
  }

  int cols = maxx - minx + 1;
  int rows = maxy + 1 + 2;

  point start_sand = {.x = 500, .y = 0};

  int **matrix = malloc(rows * sizeof(int *));

  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(cols * sizeof(int));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      point p = {.x = x + minx, .y = y};
      if (is_rock(p, paths, paths_size))
        matrix[y][x] = ROCK;
      else if (POINTS_ARE_EQUAL(p, start_sand))
        matrix[y][x] = SAND;
      else
        matrix[y][x] = AIR;
      if (y == rows - 1)
        matrix[y][x] = ROCK;
    }
  }

  start_sand.x -= minx;
  point cur_sand = start_sand;
  int rested = 0;

  int top = 0;
  while (!top) {
    while (1) {
      if (matrix[start_sand.y][start_sand.x] == REST) {
        top = 1;
        break;
      }
      if (move_sand(&cur_sand, &start_sand, &rows, &cols, &matrix) == REST) {
        rested++;
        break;
      }
    }
    cur_sand = start_sand;
  }
  printf("Units of sand rested: %d\n", rested);
  free_paths(&paths, paths_size);
  free_matrix(rows, &matrix);
  return 0;
}
