#include "main.h"
#include <ctype.h>
#include <stdio.h>

task_type **parse_task_file(FILE *task_file) {
  task_type **tasks;
  task_type task;
  char c, pc; // char and past char
  char key_buf[15], value_buf[512];
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
        printf("VALUE: %s\n", value_buf);

        expecting_value = FALSE;
        expecting_key = TRUE;
      }

      is_object = FALSE;
      goto end;
    }

    if (is_object) {
      if (c == ':' || c == ',') {
        if (expecting_key) {
          key_buf[key_buf_len] = '\0';
          key_buf_len = 0;
          printf("KEY: %s\n", key_buf);
        }

        if (expecting_value) {
          value_buf[value_buf_len] = '\0';
          value_buf_len = 0;
          printf("VALUE: %s\n", value_buf);
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

  return tasks;
}
