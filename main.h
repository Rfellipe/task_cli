#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])
#define PATH_LEN 256
#define HELP_MSG                                                               \
  "Help msg\n"                                                                 \
  "TEste\n"
#define JSON_FORMAT                                                            \
  ",\n"                                                                        \
  "  {\n"                                                                      \
  "    \"id\": %d,\n"                                                          \
  "    \"description\": \"%s\",\n"                                             \
  "    \"status\": \"%s\",\n"                                                  \
  "    \"created_at\": %ld,\n"                                                 \
  "    \"updated_at\": %ld\n"                                                  \
  "  }\n"                                                                      \
  "]\n"

typedef enum { IN_PROGRESS = 1, DONE = 2 } task_status;

struct task_type {
  int id;
  char *description;
  char status[15];
  long int created_at;
  long int updated_at;
};

struct tasks {
  struct task_type **tasks;
  int tasks_len;
};

struct tasks parse_task_file(FILE *task_file);

void panic(const char *message);

int list_tasks(const char *status_filter);

int add_task(char *task_description);

int update_task(char *task_id, char *task_description, int new_status);

int delete_task();

#endif
