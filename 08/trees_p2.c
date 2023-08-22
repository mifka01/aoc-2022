/**
 * @title Advent of Code - 08
 * @author Mifka Radim
 **/

#include <stdio.h>
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

int get_scenic_score(int row, int col, int matrix[ROWS][COLS]) {
  int tree_size = matrix[row][col];
  int up = 0;
  int down = 0;
  int left = 0;
  int right = 0;

  // up
  for (int i = row - 1; i >= 0; i--) {
    up++;
    if (tree_size <= matrix[i][col]) {
      break;
    }
  }

  // down
  for (int i = row + 1; i < ROWS; i++) {
    down++;
    if (tree_size <= matrix[i][col]) {
      break;
    }
  }

  // left
  for (int i = col - 1; i >= 0; i--) {
    left++;
    if (tree_size <= matrix[row][i]) {
      break;
    }
  }

  // right
  for (int i = col + 1; i < COLS; i++) {
    right++;
    if (tree_size <= matrix[row][i]) {
      break;
    }
  }
  return (up * down * left * right);
}

int best_scenic_score(int matrix[ROWS][COLS]) {
  int best = 0;
  int cur_tree_score;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      cur_tree_score = get_scenic_score(i, j, matrix);
      if (best < cur_tree_score)
        best = cur_tree_score;
    }
  }
  return best;
}

int main() {
  char row[MAX_LENGTH + 2];
  // add trees on edges
  int matrix[ROWS][COLS];
  int row_index = 0;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';

    for (int col = 0; row[col] != '\0'; col++) {
      matrix[row_index][col] = row[col] - '0';
    }

    row_index++;
  }

  printf("Best scenic score: %d\n", best_scenic_score(matrix));

  return 0;
}
