#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_task(struct task_type *task, char *key, char *value) {
  if (strcmp(key, "id") == 0) {
    task->id = atoi(value);

  } else if (strcmp(key, "description") == 0) {
    task->description = strdup(value);

  } else if (strcmp(key, "status") == 0) {
    snprintf(task->status, sizeof(task->status), "%s", value);

  } else if (strcmp(key, "created_at") == 0) {
    task->created_at = atol(value);

  } else if (strcmp(key, "updated_at") == 0) {
    task->updated_at = atol(value);

  } else {
    panic("Key unrecognized");
  }

  return 0;
}

struct tasks parse_task_file(FILE *task_file) {
  struct task_type **tasks = malloc(sizeof(struct task_type));
  struct task_type task = {.id = 0,
                           .description = NULL,
                           .status = {},
                           .created_at = 0,
                           .updated_at = 0};
  char c, pc; // char and past char
  char key_buf[15], value_buf[512];
  int i = 0;
  int key_buf_len = 0, value_buf_len = 0;
  int is_object = FALSE, is_value = FALSE, is_string = FALSE;
  int expecting_key = TRUE, expecting_value = FALSE;

  while ((c = fgetc(task_file)) != EOF) {
    if (!is_string && (c == ' ' || c == '\t' || c == '\n'))
      goto end;

    if (c == '{') {
      is_object = TRUE;
      goto end;
    }

    if (c == '}') {
      if (!is_string && expecting_value && value_buf_len > 0) {
        value_buf[value_buf_len] = '\0';
        value_buf_len = 0;
        parse_task(&task, key_buf, value_buf);

        expecting_value = FALSE;
        expecting_key = TRUE;
      }

      is_object = FALSE;
      tasks[i] = malloc(sizeof(struct task_type));
      *tasks[i] = task;
      i++;
      memset(&task, 0, sizeof(struct task_type));
      goto end;
    }

    if (is_object) {
      if (c == ':' || c == ',') {
        if (expecting_key) {
          key_buf[key_buf_len] = '\0';
          key_buf_len = 0;
        }

        if (expecting_value) {
          value_buf[value_buf_len] = '\0';
          value_buf_len = 0;
          parse_task(&task, key_buf, value_buf);
        }

        expecting_key = !expecting_key;
        expecting_value = !expecting_value;
        goto end;
      }

      if (isdigit(c) && !is_string) {
        value_buf[value_buf_len++] = c;
      }

      if (c == '"' && pc != '\\') {
        if (is_string) {
          is_string = FALSE;
        } else {
          is_string = TRUE;
        }
        goto end;
      }

      if (is_string) {
        if (expecting_key) {
          key_buf[key_buf_len++] = c;
        }

        if (expecting_value) {
          value_buf[value_buf_len++] = c;
        }
      }
    }
  end:
    pc = c;
  }

  struct tasks parsed_tasks = {.tasks = tasks, .tasks_len = i};

  return parsed_tasks;
}

int write_new_task_file(FILE *task_file, struct tasks tasks) {
  fflush(task_file);
  fseek(task_file, 0, SEEK_SET);

  fputs("[\n", task_file);
  for (int i = 0; i < tasks.tasks_len; i++) {
    fputs("{\"id\":", task_file);
    fprintf(task_file, "%d,", tasks.tasks[i]->id);

    fputs("\"description\":\"", task_file);
    fprintf(task_file, "%s", tasks.tasks[i]->description);
    fputs("\",", task_file);

    fputs("\"status\":\"", task_file);
    fprintf(task_file, "%s", tasks.tasks[i]->status);
    fputs("\",", task_file);

    fputs("\"created_at\":", task_file);
    fprintf(task_file, "%ld,", tasks.tasks[i]->created_at);

    fputs("\"updated_at\":", task_file);
    fprintf(task_file, "%ld", tasks.tasks[i]->updated_at);

    if (i + 1 < tasks.tasks_len) {
      fputs("},\n", task_file);
    } else {
      fputs("}\n", task_file);
    }
  }
  fputs("]\n", task_file);
  fflush(task_file);

  return 0;
}
