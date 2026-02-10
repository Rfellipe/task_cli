#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern FILE *task_file;
extern struct tasks parsed_tasks;

static void print_task(struct task_type *task) {
  printf("TASK INFO: \nid: %d\ndescription: %s\nstatus: %s\ncreated_at: "
         "%ld\tupdated_at: %ld\n\n",
         task->id, task->description, task->status, task->created_at,
         task->updated_at);
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
  int new_task_id = 0;
  char c;

  if (!task_description) {
    error(ARG_MISS, "description");
  }

  new_task_id = parsed_tasks.tasks_len // if tasks length > 0
                    ? parsed_tasks.tasks[parsed_tasks.tasks_len - 1]
                          ->id // get previous task id
                    : 0;       // else new_task_id equals 0

  struct task_type new_task = {
      .id = new_task_id + 1, // new_task_id because count start at 1
      .description = task_description,
      .status = "todo",
      .created_at = time(NULL),
      .updated_at = time(NULL),
  };

  parsed_tasks.tasks[parsed_tasks.tasks_len] = malloc(sizeof(struct task_type));
  *parsed_tasks.tasks[parsed_tasks.tasks_len] = new_task;
  parsed_tasks.tasks_len = parsed_tasks.tasks_len + 1;

  write_new_task_file(task_file, parsed_tasks);

  printf("Added task successfully (ID: %d)\n", new_task.id);

  return 0;
}

int update_task(char *task_id, char *task_description, int new_status) {
  int id;

  if (!task_id || (!task_description && !new_status)) {
    error(ARG_MISS, "Missing either task ID or task description");
  }

  if (!isdigit(task_id[0])) {
    error(ARG_ERROR, "Task ID should be a number");
  }

  id = atoi(task_id);
  struct task_type *task = parsed_tasks.tasks[id - 1];

  if (new_status) {
    // TODO: Path for mark-in-progress and mark-done
    switch (new_status) {
    case IN_PROGRESS:
      snprintf(task->status, sizeof(task->status), "%s", "in-progress");
      break;
    case DONE:
      snprintf(task->status, sizeof(task->status), "%s", "done");
      break;
    default:
      panic("Parameter not recognized");
    }
  } else {
    parsed_tasks.tasks[id - 1]->description = strdup(task_description);
  }

  write_new_task_file(task_file, parsed_tasks);

  return 0;
}

int delete_task() { return 0; }
