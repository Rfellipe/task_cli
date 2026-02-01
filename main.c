#include <asm-generic/errno-base.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define PATH_LEN 256
#define HELP_MSG                                                               \
  "Help msg\n"                                                                 \
  "TEste\n"

void help() { printf("%s", HELP_MSG); }

void panic(const char *message) {
  fprintf(stderr, "panic: %s\n", message);
  exit(1);
}

int list_tasks() { return 0; }

int add_task() { return 0; }

int update_task() { return 0; }

int remove_task() { return 0; }

int check_dir() {
  char *dir_path = malloc(PATH_LEN);
#ifdef _WIN32
  // TODO: Handle windows directory creation
  printf("Windows not handled yet");
  return 1;
#elif __linux__
  sprintf(dir_path, "%s/task_cli", getenv("HOME"));
#endif

  DIR *dir = opendir(dir_path);
  if (dir) {
    closedir(dir);
    return 0;
  } else if (ENOENT == errno) {
    int status = mkdir(dir_path, 0700);
    if (status == 0) {
      printf("Directory created\n");
      return 0;
    } else {
      printf("Error creating creating directory\n");
      return 1;
    }
  } else {
    printf("Error opening directory\n");
    return 1;
  }

  return 0;
}

int create_file() { return 0; }

int main(int argc, char **argv) {
  int err;

  if (argc == 1) {
    help();
    return 1;
  }

  if (check_dir() != 0) {
    panic("Error on directory handling\n");
  }

  return 0;
}
