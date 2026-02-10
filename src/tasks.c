#include <main.h>

extern FILE *task_file;
extern struct tasks parsed_tasks;

char *format_unix_time(long int t) {
  struct tm tm;
  char buf[32];

  localtime_r(&t, &tm);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

  return strdup(buf);
}

static void print_task(struct task_type *task) {
  printf("TASK INFO: \nid: %d\ndescription: %s\nstatus: %s\ncreated_at: "
         "%s\tupdated_at: %s\n\n",
         task->id, task->description, task->status,
         format_unix_time(task->created_at),
         format_unix_time(task->updated_at));
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

  parsed_tasks.tasks[id - 1]->updated_at = time(NULL);

  write_new_task_file(task_file, parsed_tasks);

  return 0;
}

int delete_task(char *task_id) {
  int id;

  if (!task_id) {
    error(ARG_MISS, "Missing either task ID");
  }

  if (!isdigit(task_id[0])) {
    error(ARG_ERROR, "Task ID should be a number");
  }

  id = atoi(task_id);

  for (int i = 0; i < parsed_tasks.tasks_len; i++) {
    struct task_type *task = parsed_tasks.tasks[i];

    if (task->id == id) {
      memset(parsed_tasks.tasks[i], 0, sizeof(struct task_type));
    }

    if (task->id > id) {
      task->id -= 1;
      memcpy(parsed_tasks.tasks[i - 1], task, sizeof(struct task_type));
      free(parsed_tasks.tasks[i]);
    }

    if (i + 1 == parsed_tasks.tasks_len) {
      parsed_tasks.tasks_len = i;
    }
  }

  write_new_task_file(task_file, parsed_tasks);

  return 0;
}
