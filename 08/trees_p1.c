/**
 * @title Advent of Code - 08
 * @author Mifka Radim
 **/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 200

// MATRIX SIZE
#define ROWS 99
#define COLS 99

void print_matrix(int matrix[ROWS][COLS]) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%d", matrix[i][j]);
    }
    printf("\n");
  }
}

int check_visibility(int row, int col, int matrix[ROWS][COLS]) {
  int tree_size = matrix[row][col];
  int visible = 1;

  // up
  for (int i = 0; i < row; i++) {
    if (tree_size <= matrix[i][col]) {
      visible = 0;
      break;
    }
  }

  if (visible)
    return visible;

  // down
  visible = 1;
  for (int i = row + 1; i < ROWS; i++) {
    if (tree_size <= matrix[i][col]) {
      visible = 0;
      break;
    }
  }
  if (visible)
    return visible;

  // left
  visible = 1;
  for (int i = 0; i < col; i++) {
    if (tree_size <= matrix[row][i]) {
      visible = 0;
      break;
    }
  }

  if (visible)
    return visible;

  // right
  visible = 1;
  for (int i = col + 1; i < COLS; i++) {
    if (tree_size <= matrix[row][i]) {
      visible = 0;
      break;
    }
  }

  return visible;
}

int count_visible_trees(int matrix[ROWS][COLS]) {
  int visible = 0;
  for (int i = 1; i < ROWS - 1; i++) {
    for (int j = 1; j < COLS - 1; j++) {
      if (check_visibility(i, j, matrix))
        visible++;
    }
  }
  return visible;
}

int main() {
  char row[MAX_LENGTH + 2];
  // add trees on edges
  int visible_trees = ROWS * 2 + (COLS - 2) * 2;
  int matrix[ROWS][COLS];
  int row_index = 0;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';

    for (int col = 0; row[col] != '\0'; col++) {
      matrix[row_index][col] = row[col] - '0';
    }

    row_index++;
  }

  print_matrix(matrix);
  visible_trees += count_visible_trees(matrix);

  printf("visible: %d\n", visible_trees);

  return 0;
}
