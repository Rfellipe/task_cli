#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define PATH_LEN 256
#define HELP_MSG                                                               \
  "Help msg\n"                                                                 \
  "TEste\n"

typedef enum { IN_PROGRESS, DONE, TODO } task_status;

typedef struct {
  int id;
  char *description;
  char *status;
  long int created_at;
  long int updated_at;
} task_type;

task_type **parse_task_file(FILE *task_file);

#endif
