/**
 * @title Advent of Code - 15
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define REALLOC_AMOUNT 10

#define UNKNOWN 0
#define SENSOR 1
#define BEACON 2
#define NOT_BEACON 3

#define POINTS_ARE_EQUAL(pt1, pt2) ((pt1.x == pt2.x) && (pt1.y == pt2.y))
typedef struct point {
  int value;
  int x;
  int y;
} point;

typedef struct point_arr {
  int size;
  int max_size;
  point *points;

} point_arr;

void add_point(point_arr *arr, point point) {
  if (arr->size == arr->max_size) {
    arr->points =
        realloc(arr->points, (arr->size + REALLOC_AMOUNT) * sizeof(point));
    arr->max_size += REALLOC_AMOUNT;
  }

  arr->points[arr->size] = point;
  arr->size++;
}

void init_point_array(point_arr *arr, int length) {
  arr->points = malloc(sizeof(point) * length);
  arr->size = 0;
  arr->max_size = length;
}

void free_point_array(point_arr *arr) { free(arr->points); }

void print_matrix(int rows, int cols, int **matrix) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == NOT_BEACON)
        printf("#");
      else if (matrix[i][j] == SENSOR)
        printf("S");
      else if (matrix[i][j] == BEACON)
        printf("B");
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

int point_in_array(point p, point_arr arr) {
  for (int i = 0; i < arr.size; i++) {
    if (POINTS_ARE_EQUAL(p, arr.points[i]))
      return 1;
  }
  return 0;
}

int main() {
  char row[MAX_LENGTH + 2];

  point_arr sensors;
  point_arr beacons;

  init_point_array(&sensors, REALLOC_AMOUNT);
  init_point_array(&beacons, REALLOC_AMOUNT);

  point sensor = {.value = SENSOR};
  point beacon = {.value = BEACON};

  int minx = INT_MAX;
  int maxx = INT_MIN;
  int maxy = INT_MIN;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;

    if (sscanf(row, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
               &sensor.x, &sensor.y, &beacon.x, &beacon.y) == 0) {
      return 1;
    }

    add_point(&sensors, sensor);
    add_point(&beacons, beacon);

    if (beacon.x < minx)
      minx = beacon.x;

    if (beacon.x > maxx)
      maxx = beacon.x;

    if (beacon.y > maxy)
      maxy = beacon.y;

    if (sensor.x < minx)
      minx = sensor.x;

    if (sensor.x > maxx)
      maxx = sensor.x;

    if (sensor.y > maxy)
      maxy = sensor.y;
  }

  int cols = maxx - minx + 1;
  int rows = maxy + 1;

  int **matrix = malloc(rows * sizeof(int *));

  for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      point p = {.x = x + minx, .y = y};
      if (point_in_array(p, sensors))
        matrix[y][x] = SENSOR;
      else if (point_in_array(p, beacons))
        matrix[y][x] = BEACON;
      else
        matrix[y][x] = UNKNOWN;
    }
  }

  print_matrix(rows, cols, matrix);

  free_point_array(&sensors);
  free_point_array(&beacons);

  free_matrix(rows, &matrix);

  return 0;
}
