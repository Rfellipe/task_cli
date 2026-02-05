#include "main.h"
#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// static char task_file_path[PATH_LEN];
FILE *task_file;
struct tasks parsed_tasks;

void help() { printf("%s", HELP_MSG); }

void panic(const char *message) {
  fprintf(stderr, "panic: %s\n", message);
  exit(1);
}

void print_task(struct task_type *task) {
  printf("TASK INFO: \nid: %d\ndescription: %s\nstatus: %s\ncreated_at: "
         "%ld\tupdated_at: %ld\n\n",
         task->id, task->description, task->status, task->created_at,
         task->updated_at);
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

int list_tasks(const char *status_filter) {
  for (int i = 0; i < parsed_tasks.tasks_len; i++) {
    if (!status_filter) {
      print_task(parsed_tasks.tasks[i]);
    } else {
      if (strcmp(parsed_tasks.tasks[i]->status, status_filter) == 0) {
        print_task(parsed_tasks.tasks[i]);
      }
    }
  }

  return 0;
}

int add_task(char *task_description) {
  int i = 0;
  char c;
  int last_task_id = parsed_tasks.tasks[parsed_tasks.tasks_len - 1]->id;
  struct task_type new_task = {
      .id = last_task_id += 1,
      .description = task_description,
      .status = "todo",
      .created_at = time(NULL),
      .updated_at = time(NULL),
  };

  fseek(task_file, i, SEEK_END);
  while ((c = fgetc(task_file)) != '}') {
    fseek(task_file, i--, SEEK_END);
  }

  fprintf(task_file, JSON_FORMAT, new_task.id, new_task.description,
          new_task.status, new_task.created_at, new_task.updated_at);

  printf("Added task successfully (ID: %d)\n", new_task.id);

  return 0;
}

int update_task() { return 0; }

int delete_task() { return 0; }

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
    update_task();

  } else if (strcmp(argv[1], "delete") == 0) {
    delete_task();

  } else if (strcmp(argv[1], "mark-in-progress") == 0) {
    update_task();

  } else if (strcmp(argv[1], "mark-done") == 0) {
    update_task();

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
