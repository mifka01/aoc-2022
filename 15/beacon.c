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

#define MAX_LENGTH 500
#define REALLOC_AMOUNT 10

#define POINTS_ARE_EQUAL(pt1, pt2) ((pt1.x == pt2.x) && (pt1.y == pt2.y))

typedef struct point {
  int x;
  int y;
  int distance;
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

int distance(point p1, point p2) { return abs(p1.x - p2.x) + abs(p1.y - p2.y); }

int point_in_array(point p, point_arr arr) {
  for (int i = 0; i < arr.size; i++) {
    if (POINTS_ARE_EQUAL(p, arr.points[i]))
      return 1;
  }
  return 0;
}

int main() {
  char row[MAX_LENGTH + 2];

  point sensor;
  point beacon;

  int minx = INT_MAX;
  int maxx = INT_MIN;

  point_arr devices;
  init_point_array(&devices, REALLOC_AMOUNT);

  point p = {.x = 0, .y = 2000000};

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;

    if (sscanf(row, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
               &sensor.x, &sensor.y, &beacon.x, &beacon.y) == 0) {
      return 1;
    }

    if (sensor.y == p.y && !point_in_array(sensor, devices))
      add_point(&devices, sensor);

    if (beacon.y == p.y && !point_in_array(beacon, devices))
      add_point(&devices, beacon);

    sensor.distance = distance(sensor, beacon);

    if (p.y >= sensor.y - sensor.distance &&
        p.y <= sensor.y + sensor.distance) {
      for (int x = sensor.x - sensor.distance; x < sensor.x + sensor.distance;
           x++) {
        p.x = x;
        if (distance(p, sensor) <= sensor.distance &&
            !POINTS_ARE_EQUAL(p, sensor) && !POINTS_ARE_EQUAL(p, beacon)) {
          if (p.x > maxx)
            maxx = p.x;
          if (p.x < minx)
            minx = p.x;
        }
      }
    }
  }
  printf("%d\n", abs(minx - maxx) + 1 - devices.size);

  free_point_array(&devices);
  return 0;
}
