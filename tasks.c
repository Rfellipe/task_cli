#include "main.h"
#include <ctype.h>
#include <time.h>

extern FILE *task_file;
extern struct tasks parsed_tasks;

void print_task(struct task_type *task) {
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

int update_task(char *task_id, char *task_description, int new_status) {
  // task-cli update 1 "new description"
  if (!task_id || !task_description) {
    printf("Insuficient arguments\n");
    return 1;
  }

  if (!isdigit(task_id)) {
    printf("Task ID should be a number\n");
    return 1;
  }

  if (new_status) {
    // TODO: Path for mark-in-progress and mark-done
    switch (new_status) {
    case IN_PROGRESS:
      break;
    case DONE:
      break;
    }

    return 0;
  }

  return 0;
}

int delete_task() { return 0; }
