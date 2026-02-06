#include "main.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

FILE *task_file;
struct tasks parsed_tasks;

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

int main(int argc, char **argv) {
  int err, task_id;

  if (argc == 1) {
    help();
    return 1;
  }

  if (check_task_directory() != 0) {
    panic("Error on directory handling");
  }

  parsed_tasks = parse_task_file(task_file);

  if (strcmp(argv[1], "add") == 0) {
    if (!argv[2]) {
      printf("Need a task name\n");
      help();
      return 1;
    }

    add_task(argv[2]);

  } else if (strcmp(argv[1], "update") == 0) {
    update_task(argv[2], argv[3], 0);

  } else if (strcmp(argv[1], "delete") == 0) {
    delete_task();

  } else if (strcmp(argv[1], "mark-in-progress") == 0) {
    update_task(argv[2], NULL, IN_PROGRESS);

  } else if (strcmp(argv[1], "mark-done") == 0) {
    update_task(argv[2], NULL, DONE);

  } else if (strcmp(argv[1], "list") == 0) {
    if (argv[2]) {
      if (strcmp(argv[2], "done") == 0) {
        list_tasks("done");

      } else if (strcmp(argv[2], "todo") == 0) {
        list_tasks("todo");

      } else if (strcmp(argv[2], "in-progress") == 0) {
        list_tasks("in-progress");

      } else {
        panic("Argument not known");
      }
    } else {
      list_tasks(NULL);
    }
  } else {
    panic("Argument not known");
  }

  fclose(task_file);

  return 0;
}
