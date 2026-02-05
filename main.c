#include "main.h"
#include <asm-generic/errno-base.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// static char task_file_path[PATH_LEN];
FILE *task_file;

void help() { printf("%s", HELP_MSG); }

void panic(const char *message) {
  fprintf(stderr, "panic: %s\n", message);
  exit(1);
}

int check_task_directory() {
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
  } else if (ENOENT == errno) {
    int status = mkdir(dir_path, 0700);
    if (status != 0) {
      printf("Error creating creating directory\n");
      return 1;
    }
  } else {
    printf("Error opening directory\n");
    return 1;
  }

  strcat(dir_path, "/tasks.json");
create_file:
  task_file = fopen(dir_path, "r+");
  if (task_file != NULL) {
    // fclose(file);
    // strcpy(task_file_path, dir_path);
    free(dir_path);
  } else {
    task_file = fopen(dir_path, "w+");
    fprintf(task_file, "[]\n");
    fclose(task_file);
    goto create_file;
    return 1;
  }

  return 0;
}

int list_tasks() {
  struct tasks parsed_tasks = parse_task_file(task_file);

  for (int i = 0; i < parsed_tasks.tasks_len; i++) {
    printf("TASK INFO: \nid: %d\ndescription: %s\nstatus: %s\ncreated_at: "
           "%ld\tupdated_at: %ld\n\n",
           parsed_tasks.tasks[i]->id, parsed_tasks.tasks[i]->description,
           parsed_tasks.tasks[i]->status, parsed_tasks.tasks[i]->created_at,
           parsed_tasks.tasks[i]->updated_at);
  }

  return 0;
}

int add_task() { return 0; }

int update_task() { return 0; }

int delete_task() { return 0; }

int main(int argc, char **argv) {
  int err;
  //   char *dir_path = malloc(PATH_LEN);
  // #ifdef _WIN32
  //   // TODO: Handle windows directory creation
  //   printf("Windows not handled yet");
  //   return 1;
  // #elif __linux__
  //   sprintf(dir_path, "%s/task_cli", getenv("HOME"));
  // #endif

  if (argc == 1) {
    help();
    return 1;
  }

  if (check_task_directory() != 0) {
    panic("Error on directory handling");
  }

  if (!strcmp(argv[1], "add")) {
    add_task();
  } else if (!strcmp(argv[1], "update")) {
    update_task();
  } else if (!strcmp(argv[1], "delete")) {
    delete_task();
  } else if (!strcmp(argv[1], "mark-in-progress")) {
    update_task(); // mark-in-progress
  } else if (!strcmp(argv[1], "mark-done")) {
    update_task(); // mark-done
  } else if (!strcmp(argv[1], "list")) {
    list_tasks();
  } else {
    printf("Parameter not known\n");
    return 1;
  }

  return 0;
}
