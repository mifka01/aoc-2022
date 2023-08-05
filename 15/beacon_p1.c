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

#define MAX_COORDINATE 4000000

#define POINTS_ARE_EQUAL(pt1, pt2) ((pt1.x == pt2.x) && (pt1.y == pt2.y))

typedef struct point {
  int x;
  int y;
  int distance;
  struct point *beacon;
} point;

typedef struct point_arr {
  int size;
  int max_size;
  point *points;

} point_arr;

typedef struct g_row {
  int y;
  int blocked;
} g_row;

typedef struct range {
  int from;
  int to;
} range;

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

void free_point_array(point_arr *arr) {
  for (int i = 0; i < arr->size; i++) {
    free(arr->points[i].beacon);
  }
  free(arr->points);
}

int distance(point p1, point p2) { return abs(p1.x - p2.x) + abs(p1.y - p2.y); }

int is_beacon(point p, point_arr arr) {
  for (int i = 0; i < arr.size; i++) {
    point beacon = *(arr.points[i].beacon);
    if (POINTS_ARE_EQUAL(p, beacon))
      return 1;
  }
  return 0;
}

void print_point_array(point_arr arr) {
  for (int i = 0; i < arr.size; i++) {
    printf("%d %d\n", arr.points[i].x, arr.points[i].y);
  }
}

int compare_ranges(const void *a, const void *b) {
  return ((range *)a)->from - ((range *)b)->from;
}

void print_ranges(range *ranges, int ranges_size) {
  for (int i = 0; i < ranges_size; i++) {
    printf("%d %d\n", ranges[i].from, ranges[i].to);
  }
}

void merge_ranges(range **ranges, int *ranges_size) {
  qsort(*ranges, *ranges_size, sizeof(range), compare_ranges);

  range merged_ranges[*ranges_size + 1];
  int merged_count = 0;

  merged_ranges[0].from = (*ranges)[0].from;
  merged_ranges[0].to = (*ranges[0]).to;

  for (int i = 1; i < *ranges_size; i++) {
    if ((*ranges)[i].from <= merged_ranges[merged_count].to + 1) {
      if ((*ranges)[i].to > merged_ranges[merged_count].to) {
        merged_ranges[merged_count].to = (*ranges)[i].to;
      }
    } else {
      merged_count++;
      merged_ranges[merged_count].from = (*ranges)[i].from;
      merged_ranges[merged_count].to = (*ranges)[i].to;
    }
  }

  for (int i = 0; i <= merged_count; i++) {
    (*ranges)[i] = merged_ranges[i];
  }
  *ranges_size = merged_count + 1;
}

void get_row(g_row *row, point_arr sensors, int y) {

  int minx = INT_MAX;
  int maxx = INT_MIN;

  int sensors_on_y = 0;

  row->y = y;
  row->blocked = 0;

  int max_ranges = REALLOC_AMOUNT;
  int ranges_size = 0;
  range *ranges = malloc(max_ranges * sizeof(range));

  point p = {.y = y};
  for (int i = 0; i < sensors.size; i++) {
    point sensor = sensors.points[i];
    point beacon = *(sensor.beacon);

    if (p.y == sensor.y)
      sensors_on_y++;

    if (p.y >= sensor.y - sensor.distance &&
        p.y <= sensor.y + sensor.distance) {
      for (p.x = sensor.x - sensor.distance; p.x < sensor.x + sensor.distance;
           p.x++) {
        if (distance(p, sensor) <= sensor.distance &&
            !POINTS_ARE_EQUAL(p, sensor) && !POINTS_ARE_EQUAL(p, beacon)) {
          if (p.x > maxx)
            maxx = p.x;
          if (p.x < minx)
            minx = p.x;
        }
      }
      if (!(minx == INT_MAX && maxx == INT_MIN)) {
        if (ranges_size == max_ranges) {
          max_ranges += REALLOC_AMOUNT;
          ranges = realloc(ranges, max_ranges * sizeof(range));
        }
        range rng = {.from = minx, .to = maxx};
        ranges[ranges_size] = rng;
        ranges_size++;
        minx = INT_MAX;
        maxx = INT_MIN;
      }
    }
  }
  merge_ranges(&ranges, &ranges_size);

  for (int i = 0; i < ranges_size; i++) {
    row->blocked += ranges[i].to - ranges[i].from + 1;
  }
  row->blocked -= sensors_on_y;

  free(ranges);
}

int main() {
  char row[MAX_LENGTH + 2];

  point sensor;

  point_arr sensors;
  init_point_array(&sensors, REALLOC_AMOUNT);

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      continue;

    point *beacon = (point *)malloc(sizeof(point));
    if (beacon == NULL) {
      return 1;
    }

    if (sscanf(row, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
               &sensor.x, &sensor.y, &beacon->x, &beacon->y) == 0) {
      return 1;
    }

    sensor.distance = distance(sensor, *beacon);
    sensor.beacon = beacon;

    add_point(&sensors, sensor);
  }

  g_row r;
  get_row(&r, sensors, 2000000);

  printf("Blocked points: %d\n", r.blocked);

  free_point_array(&sensors);
  return 0;
}
