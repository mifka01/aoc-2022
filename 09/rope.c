/**
 * @title Advent of Code - 09
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define MAX_TAIL_POSITIONS 1024
#define MAX_KNOTS 10
#define REALLOC_AMOUNT 50

typedef struct point {
  int x;
  int y;
} point;

typedef struct point_arr {
  int size;
  int max_size;
  point *points;

} point_arr;

int is_neighbor(point a, point b) {
  int dx = abs(a.x - b.x);
  int dy = abs(a.y - b.y);

  return ((dx <= 1 && dy <= 1));
}

int point_visited(point cur_point, point_arr *arr) {
  for (int i = 0; i < arr->size; i++) {
    if (arr->points[i].x == cur_point.x && arr->points[i].y == cur_point.y)
      return 1;
  }
  return 0;
}

void init_knots(point *arr, int knots_len) {
  for (int i = 0; i < knots_len; i++) {
    arr[i].x = 0;
    arr[i].y = 0;
  }
}

void print_knots(point *arr, int knots_len) {
  for (int knot_i = 0; knot_i < knots_len; knot_i++) {
    printf("knot: %d, x: %d, y: %d\n", knot_i, arr[knot_i].x, arr[knot_i].y);
  }
}

int move_tails(point **knots, int knots_len) {
  for (int i = 1; i < knots_len; i++) {
    if (!is_neighbor((*knots)[i - 1], (*knots)[i])) {
      if ((*knots)[i - 1].x != (*knots)[i].x)
        (*knots)[i].x += ((*knots)[i - 1].x < (*knots)[i].x) ? -1 : 1;

      if ((*knots)[i - 1].y != (*knots)[i].y)
        (*knots)[i].y += ((*knots)[i - 1].y < (*knots)[i].y) ? -1 : 1;

      if (i == knots_len - 1)
        return 1;
    }
  }
  return 0;
}

void direction_to_delta(const char *direction, int *dx, int *dy) {
  switch (direction[0]) {
  case 'R':
    *dx = 1;
    *dy = 0;
    break;
  case 'L':
    *dx = -1;
    *dy = 0;
    break;
  case 'U':
    *dx = 0;
    *dy = 1;
    break;
  case 'D':
    *dx = 0;
    *dy = -1;
    break;
  default:
    *dx = 0;
    *dy = 0;
    break;
  }
}

void add_point(point_arr *arr, point point) {
  if (arr->size == arr->max_size) {
    arr->points =
        realloc(arr->points, (arr->size + REALLOC_AMOUNT) * sizeof(point));
    arr->max_size += REALLOC_AMOUNT;
  }

  arr->points[arr->size] = point;
  arr->size++;
}

point_arr create_point_array(int length) {
  point_arr arr;
  arr.points = malloc(sizeof(point) * length);
  arr.size = 0;
  arr.max_size = length;
  return arr;
}

void clear_point_array(point_arr *arr) { free(arr->points); }

int main() {
  char row[MAX_LENGTH + 2];

  point_arr tail_positions = create_point_array(MAX_TAIL_POSITIONS);
  point start = {.x = 0, .y = 0};
  add_point(&tail_positions, start);

  point *knots = malloc(sizeof(point) * MAX_KNOTS);
  init_knots(knots, MAX_KNOTS);

  int dx, dy;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';

    if (strlen(row) < 2)
      continue;

    direction_to_delta(row, &dx, &dy);

    if (dx == 0 && dy == 0) {
      continue;
    }
    for (int i = 0; i < atoi(row + 2); i++) {
      knots[0].x += dx;
      knots[0].y += dy;
      if (move_tails(&knots, MAX_KNOTS) &&
          !point_visited(knots[MAX_KNOTS - 1], &tail_positions)) {
        add_point(&tail_positions, knots[MAX_KNOTS - 1]);
      }
    }
  }
  printf("head: %d,%d tail: %d,%d\n", knots[0].x, knots[0].y,
         knots[MAX_KNOTS - 1].x, knots[MAX_KNOTS - 1].y);
  printf("tail visited: %d\n", tail_positions.size);

  free(knots);
  clear_point_array(&tail_positions);
  return 0;
}
