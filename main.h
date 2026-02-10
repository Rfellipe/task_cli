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
  "Help msg\n"                                                                 \
  "TEste\n"

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
