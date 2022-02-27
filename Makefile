.PHONY: all clean test

PROG=   fchmodexec
SRCS=   fchmodexec.c

CFLAGS ?= -D_FORTIFY_SOURCE=2 -O2 -fstack-protector-strong \
					-Wformat -Werror=format-security \
					-pie -fPIE \
					-fno-strict-aliasing \
					-g -Wall -fwrapv -pedantic
LDFLAGS ?= -Wl,-z,relro,-z,now -Wl,-z,noexecstack

CFLAGS += $(FCHMODEXEC_CFLAGS)
LDFLAGS += $(FCHMODEXEC_LDFLAGS)

$(PROG):
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LDFLAGS)

all: $(PROG) lib

lib:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
    -Wconversion -Wshadow \
    -Wpointer-arith -Wcast-qual \
    -Wstrict-prototypes -Wmissing-prototypes \
    -o libdisablefchmod.so libdisablefchmod.c -ldl \
    -Wl,-z,relro,-z,now -Wl,-z,noexecstack

clean:
	-@rm $(PROG) libdisablefchmod.so

test: $(PROG)
	@PATH=.:$(PATH) bats test
