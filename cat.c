#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_CAP (1024 * 8)
uint8_t buf[BUF_CAP];

#define STDIN_DYNAMIC 0
#if STDIN_DYNAMIC
void cat_stdin(void) {
  size_t buf_size = 128;
  size_t buf_cursor_pos = 0;
  char *stdin_buf = malloc(buf_size);
  while (true) {
    if (buf_size - buf_cursor_pos < 10) {
      stdin_buf = realloc(stdin_buf, buf_size + 128);
      buf_size += 128;
    }
    int ch = getchar();
    if (ch == EOF) {
      printf("%.*s\n", (int)buf_cursor_pos, stdin_buf);
      return;
    } else if (ch == '\n') {
      printf("%.*s\n", (int)buf_cursor_pos, stdin_buf);
      buf_cursor_pos = 0;
    } else {
      stdin_buf[buf_cursor_pos++] = ch;
    }
  }
}
#else
void cat_stdin(void) {
  size_t buf_cursor_pos = 0;
  while (true) {
    if (BUF_CAP - buf_cursor_pos == 0) {
      printf("%.*s\n", (int)buf_cursor_pos, buf);
      buf_cursor_pos = 0;
    }
    int ch = getchar();
    if (ch == EOF) {
      printf("%.*s\n", (int)buf_cursor_pos, buf);
      return;
    } else if (ch == '\n') {
      printf("%.*s\n", (int)buf_cursor_pos, buf);
      buf_cursor_pos = 0;
    } else {
      buf[buf_cursor_pos++] = ch;
    }
  }
}
#endif

void cat_file(const char *filename) {
  if (strcmp(filename, "-") == 0) {
    cat_stdin();
    return;
  }
  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    fprintf(stderr,
            "An error occurred while trying to open file `%s`: %s. Aborting.\n",
            filename, strerror(errno));
    exit(1);
  }

  size_t bytes_read;
  do {
    bytes_read = fread(buf, sizeof(char), BUF_CAP, fd);
    printf("%.*s", (int)bytes_read, buf);
  } while (bytes_read == BUF_CAP);
}

int main(int argc, char **argv) {
  if (argc == 1)
    cat_stdin();

  for (int i = 1; i < argc; ++i) {
    cat_file(argv[i]);
  }
}
