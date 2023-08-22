/**
 * @title Advent of Code - 07
 * @author Mifka Radim
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBDIRS 5
#define REALLOC_AMOUNT 5
#define MAX_LENGTH 50
#define AT_MOST 100000
#define FILESYSTEM_SIZE 70000000
#define NEEDED_SPACE 30000000

typedef struct dir {
  char *name;
  int size;
  struct dir **dirs;
  int dirs_len;
  int max_subdirs;
} dir;

void print_dirs(dir *arr, int len_dirs) {
  for (int i = 0; i < len_dirs; i++) {
    printf("dir name: %s, size: %d\n", arr[i].name, arr[i].size);
  }
}

dir *get_dir(char *name, dir *arr, int len_dirs) {
  for (int i = 0; i < len_dirs; i++) {
    if (strcmp(name, arr[i].name) == 0)
      return &arr[i];
  }
  return NULL;
}

void init_dir(dir *new_dir, char *name, int size, dir *dirs, int dirs_len) {
  int name_len = strlen(name);

  new_dir->name = calloc(name_len + 1, sizeof(char));
  new_dir->dirs = malloc(sizeof(dir) * MAX_SUBDIRS);
  new_dir->dirs_len = dirs_len;
  new_dir->size = size;
  new_dir->max_subdirs = MAX_SUBDIRS;

  memcpy(new_dir->name, name, name_len);

  for (int i = 0; i < dirs_len; i++) {
    new_dir->dirs[i] = &dirs[i];
  }
}

void add_subdir(dir *cur_dir, dir *subdir) {
  if (cur_dir->dirs_len == cur_dir->max_subdirs) {
    cur_dir->dirs = realloc(cur_dir->dirs,
                            (cur_dir->dirs_len + REALLOC_AMOUNT) * sizeof(dir));
    cur_dir->max_subdirs += REALLOC_AMOUNT;
  }

  cur_dir->dirs[cur_dir->dirs_len] = subdir;
  cur_dir->dirs_len++;
}

int calculate_subdirs(dir *current_dir) {

  int total_size = current_dir->size;

  for (int i = 0; i < current_dir->dirs_len; i++) {
    total_size += calculate_subdirs(current_dir->dirs[i]);
  }

  current_dir->size = total_size;

  return total_size;
}

void add_to_path(char **path, char *dir) {
  if (strncmp(dir, "..", 2) == 0) {
    (*path)[strlen(*path) - 1] = '\0';
    char *e;
    int index;
    e = strrchr(*path, '/');
    index = (int)(e - *path);
    (*path)[index] = '\0';
  } else {
    strncat(*path, dir, strlen(dir));
  }
  if (strcmp(*path, "/") != 0)
    strcat(*path, "/");
}

char *get_fullpath(char *path, char *name) {
  char *dirname = malloc(sizeof(char) * (strlen(path) + strlen(name) + 1));
  strcpy(dirname, path);
  strcat(dirname, name);

  return dirname;
}

void clear_dir(dir *cur_dir) {
  for (int i = 0; i < cur_dir->dirs_len; i++) {
    clear_dir(cur_dir->dirs[i]);
  }
  free(cur_dir->name);
  free(cur_dir->dirs);
}

int main() {

  char row[MAX_LENGTH + 2];
  dir arr[500];
  char *path = calloc(100, sizeof(char));
  int dirs = 1;
  init_dir(&arr[0], "/", 0, NULL, 0);
  dir *current_dir;
  int ls = 0;

  while (fgets(row, MAX_LENGTH + 2, stdin) != NULL) {
    row[strlen(row) - 1] = '\0';
    if (strlen(row) == 0)
      break;

    if (strncmp(row, "$ cd", 4) == 0) {
      ls = 0;
      if (strncmp(row + 5, "..", 2) == 0) {
        add_to_path(&path, row + 5);
        continue;
      }

      char *dirname = get_fullpath(path, row + 5);
      current_dir = get_dir(dirname, arr, dirs);
      free(dirname);
      add_to_path(&path, row + 5);

    } else if (strncmp(row, "dir", 3) == 0) {

      char *dirname = get_fullpath(path, row + 4);
      init_dir(&arr[dirs], dirname, 0, NULL, 0);
      free(dirname);

      if (ls)
        add_subdir(current_dir, &arr[dirs]);

      dirs++;
    } else if (strncmp(row, "$ ls", 3) == 0) {
      ls = 1;
    } else {
      if (ls) {
        int size;
        sscanf(row, "%d", &size);
        current_dir->size += size;
      }
    }
  }
  calculate_subdirs(&arr[0]);

  int unused_space = FILESYSTEM_SIZE - arr[0].size;
  int needed_space = NEEDED_SPACE - unused_space;
  int best = arr[0].size;

  for (int i = 0; i != dirs; i++) {
    if (arr[i].size < best && arr[i].size >= needed_space) {
      best = arr[i].size;
    }
  }
  printf("Size of dir that we want to delete is: %d\n", best);

  free(path);
  clear_dir(&arr[0]);

  return 0;
}
