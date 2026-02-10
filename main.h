#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
#define PATH_LEN 256
#define HELP_MSG                                                               \
  "task_cli — CLI Task Tracker\n\n"                                            \
  "USAGE:\n"                                                                   \
  "  task_cli <command>\n\n"                                                   \
  "COMMANDS:\n\n"                                                              \
  "  add <description>\n"                                                      \
  "      Add a new task\n"                                                     \
  "      Example:\n"                                                           \
  "        task_cli add \"Buy groceries\"\n\n"                                 \
  "  update <id> <description>\n"                                              \
  "      Update the description of an existing task\n"                         \
  "      Example:\n"                                                           \
  "        task_cli update 1 \"Buy groceries and cook dinner\"\n\n"            \
  "  delete <id>\n"                                                            \
  "      Delete a task by its ID\n"                                            \
  "      Example:\n"                                                           \
  "        task_cli delete 1\n\n"                                              \
  "  mark-in-progress <id>\n"                                                  \
  "      Mark a task as in progress\n"                                         \
  "      Example:\n"                                                           \
  "        task_cli mark-in-progress 1\n\n"                                    \
  "  mark-done <id>\n"                                                         \
  "      Mark a task as done\n"                                                \
  "      Example:\n"                                                           \
  "        task_cli mark-done 1\n\n"                                           \
  "  list\n"                                                                   \
  "      List all tasks\n\n"                                                   \
  "  list todo\n"                                                              \
  "  list in-progress\n"                                                       \
  "  list done\n\n"                                                            \
  "NOTES:\n"                                                                   \
  "  - Task IDs are numeric and assigned automatically\n"                      \
  "  - Descriptions containing spaces must be quoted\n"

typedef enum { IN_PROGRESS = 1, DONE } task_status;
typedef enum {
  ARG_MISS = 1,
  ARG_UNKOWN,
  ARG_ERROR,
} errors;

struct task_type {
  int id;
  char *description;
  char status[15];
  time_t created_at;
  time_t updated_at;
};

struct tasks {
  struct task_type **tasks;
  int tasks_len;
};

struct tasks parse_task_file(FILE *task_file);

int write_new_task_file(FILE *task_file, struct tasks tasks);

char *get_directory_path();

void panic(const char *message);

void error(errors error, const char *message);

void clear_file();

int list_tasks(const char *status_filter);

int add_task(char *task_description);

int update_task(char *task_id, char *task_description, int new_status);

int delete_task(char *task_id);

#endif
