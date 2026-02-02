#ifndef MAIN_H
#define MAIN_H

#define PATH_LEN 256

#define HELP_MSG                                                               \
  "Help msg\n"                                                                 \
  "TEste\n"

typedef enum { IN_PROGRESS, DONE, TODO } task_status;

struct task {
  int id;
  char *description;
  task_status status;
  int created_at;
  int updated_at;
};

#endif
