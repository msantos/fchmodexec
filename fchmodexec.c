/*
 * Copyright (c) 2021, Michael Santos <michael.santos@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

#include <string.h>
#include <sys/stat.h>

#define FCHMODEXEC_VERSION "0.1.1"

static void usage(void);

extern char *__progname;

int main(int argc, char *argv[]) {
  mode_t mode;
  int i;
  int fd;
  int found = 0;
  char *endptr;

  /* 0: progname
   * 1: mode
   * 2..n: fd <..>
   * --: end of options
   * args
   */
  if (argc < 3)
    usage();

  errno = 0;
  mode = (mode_t)strtol(argv[1], &endptr, 8);

  if (errno != 0)
    err(111, "strtol: %s", argv[1]);

  if (endptr == argv[1] || *endptr != '\0')
    usage();

  for (i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "--")) {
      found = 1;
      break;
    }

    errno = 0;
    fd = (mode_t)strtol(argv[i], &endptr, 10);

    if (errno != 0)
      err(111, "strtol: %s", argv[i]);

    if (endptr == argv[i] || *endptr != '\0' || fd < 0)
      usage();

    if (fchmod(fd, mode) == -1) {
      warn("fchmod(%d, %o)", fd, mode);
    }
  }

  i++;

  if (found == 0 || argc == i) {
    usage();
  }

  (void)execvp(argv[i], argv + i);

  err(127, "%s", argv[i]);
}

static void usage(void) {
  (void)fprintf(stderr,
                "%s %s\n"
                "usage: %s <mode> <fd> <...> -- <cmd> <...>\n",
                __progname, FCHMODEXEC_VERSION, __progname);
  exit(1);
}
