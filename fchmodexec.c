/* Copyright (c) 2021-2022, Michael Santos <michael.santos@gmail.com>
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
#include <stdnoreturn.h>
#include <sys/resource.h>
#include <unistd.h>

#include <string.h>
#include <sys/stat.h>

#define FCHMODEXEC_VERSION "0.3.2"

static noreturn void usage(void);
static int fdloop(int *argc, char *argv[], mode_t mode, int apply);

extern char *__progname;

int main(int argc, char *argv[]) {
  mode_t mode;
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

  if (fdloop(&argc, argv, mode, 0) == -1)
    usage();

  if (fdloop(&argc, argv, mode, 1) == -1)
    exit(111);

  (void)execvp(argv[argc], argv + argc);

  err(errno == ENOENT ? 127 : 126, "%s", argv[argc]);
}

static int fdloop(int *argc, char *argv[], mode_t mode, int apply) {
  int i;
  int fd;
  int found = 0;
  char *endptr;

  for (i = 2; i < *argc; i++) {
    if (!strcmp(argv[i], "--")) {
      found = 1;
      break;
    }

    errno = 0;
    fd = (int)strtol(argv[i], &endptr, 10);

    if (errno != 0) {
      warn("strtol: %s", argv[i]);
      return -1;
    }

    if (endptr == argv[i] || *endptr != '\0' || fd < 0) {
      warnx("invalid argument: %s", argv[i]);
      return -1;
    }

    if (!apply)
      continue;

    if (fcntl(fd, F_GETFD, 0) == -1)
      continue;

    if (fchmod(fd, mode) == -1) {
      warn("fchmod(%d, %o)", fd, mode);
      return -1;
    }
  }

  i++;

  if (found == 0 || *argc == i) {
    return -1;
  }

  if (apply)
    *argc = i;

  return 0;
}

static void usage(void) {
  (void)fprintf(stderr,
                "%s %s\n"
                "usage: %s <mode> <fd> <...> -- <cmd> <...>\n",
                __progname, FCHMODEXEC_VERSION, __progname);
  exit(2);
}
