/**
 * @title Advent of Code - 15
 * @author Mifka Radim
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 500
#define REALLOC_AMOUNT 10

#define MAX_COORDINATE 4000000

#define POINTS_ARE_EQUAL(pt1, pt2) ((pt1.x == pt2.x) && (pt1.y == pt2.y))

#define IN_AREA(border_point)                                                  \
  ((border_point.x >= 0 && border_point.x <= MAX_COORDINATE) &&                \
   (border_point.y >= 0 && border_point.y <= MAX_COORDINATE))

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

int not_blocked(point p, point_arr sensors) {

  for (int i = 0; i < sensors.size; i++) {
    if (distance(p, sensors.points[i]) <= sensors.points[i].distance) {
      return 0;
    }
  }
  return 1;
}

long long int calculate_tuning_frequency(point p) {
  return (long long int)p.x * (long long int)MAX_COORDINATE + p.y;
}

int traverse_border(point top, point bottom, point left, point right,
                    point_arr sensors) {

  point current = top;
  point border_point = {.x = 0, .y = 0};
  while (!POINTS_ARE_EQUAL(current, left)) {
    if (POINTS_ARE_EQUAL(current, top)) {
      border_point.x = current.x + 1;
      border_point.y = current.y;
      if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
        printf("Tuning frequency: %lld\n",
               calculate_tuning_frequency(border_point));
        return 1;
      }
    }

    border_point.x = current.x - 1;
    border_point.y = current.y;
    if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
      printf("Tuning frequency: %lld\n",
             calculate_tuning_frequency(border_point));

      return 1;
    }

    current.x--;
    current.y++;
  }

  while (!POINTS_ARE_EQUAL(current, bottom)) {
    border_point.x = current.x - 1;
    border_point.y = current.y;
    if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
      printf("Tuning frequency: %lld\n",
             calculate_tuning_frequency(border_point));

      return 1;
    }
    current.x++;
    current.y++;
  }

  while (!POINTS_ARE_EQUAL(current, right)) {
    if (POINTS_ARE_EQUAL(current, bottom)) {
      border_point.x = current.x + 1;
      border_point.y = current.y;
      if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
        printf("Tuning frequency: %lld\n",
               calculate_tuning_frequency(border_point));

        return 1;
      }
    }

    border_point.x = current.x - 1;
    border_point.y = current.y;
    if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
      printf("Tuning frequency: %lld\n",
             calculate_tuning_frequency(border_point));

      return 1;
    }
    current.x++;
    current.y--;
  }

  while (!POINTS_ARE_EQUAL(current, top)) {
    border_point.x = current.x - 1;
    border_point.y = current.y;
    if (IN_AREA(border_point) && not_blocked(border_point, sensors)) {
      printf("Tuning frequency: %lld\n",
             calculate_tuning_frequency(border_point));

      return 1;
    }
    current.x--;
    current.y--;
  }

  return 0;
}

int main() {
  char row[MAX_LENGTH + 2];

  point sensor;
  point beacon;

  point_arr sensors;
  init_point_array(&sensors, REALLOC_AMOUNT);

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;

    if (sscanf(row, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
               &sensor.x, &sensor.y, &beacon.x, &beacon.y) == 0) {
      return 1;
    }

    sensor.distance = distance(sensor, beacon);

    add_point(&sensors, sensor);
  }

  for (int i = 0; i < sensors.size; i++) {

    int x = sensors.points[i].x;
    int y = sensors.points[i].y;
    int distance = sensors.points[i].distance;

    point top = {.x = x, .y = y - distance};
    point bottom = {.x = x, .y = y + distance};
    point left = {.x = x - distance, .y = y};
    point right = {.x = x + distance, .y = y};
    if (traverse_border(top, bottom, left, right, sensors)) {
      break;
    }
  }
  free_point_array(&sensors);
  return 0;
}
