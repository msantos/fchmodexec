/* MIT License
 *
 * Copyright (c) 2021-2022 Michael Santos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <dlfcn.h>
#include <stdio.h>
#include <sys/stat.h>

void _init(void);
int (*sys_fchmod)(int fd, mode_t mode);
#pragma GCC diagnostic ignored "-Wpedantic"
int fchmod(int fd, mode_t mode);
#pragma GCC diagnostic warning "-Wpedantic"

void _init(void) {
  const char *err;
#pragma GCC diagnostic ignored "-Wpedantic"
  sys_fchmod = dlsym(RTLD_NEXT, "fchmod");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL) {
    (void)fprintf(stderr, "libdisablefchmod:dlsym (fchmod):%s\n", err);
  }
}

int fchmod(int fd, mode_t mode) {
  switch (fd) {
  case 1:
  case 2:
    return 0;
  default:
    break;
  }
  return sys_fchmod(fd, mode);
}
