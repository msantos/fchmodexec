# SYNOPSIS

fchmodexec *mode* *fd* *...* -- *cmd* *...*

LD_PRELOAD=libdisablefchmod.so *command* *...*

# DESCRIPTION

fchmodexec - fchmod(2) inherited file descriptors before exec(3)'ing
a command

`fchmodexec` runs as part of an exec chain to change the permissions of
any file descriptors inherited from the parent process before executing
a program.

libdisablefchmod: Disables calls fchmod(2) to stdin and stdout by
dynamically linked executables.

# EXAMPLES

## fchmodexec

```
$ umask 077; (fchmodexec 644 1 -- ls -al out) > out
$ cat out
-rw-r--r-- 1 msantos msantos 0 Mar 18 07:32 out
$ ls -al out
-rw-r--r-- 1 msantos msantos 48 Mar 18 07:32 out
```

## libdisablefchmod

```
$ umask 022; (LD_PRELOAD=libdisablefchmod.so fchmodexec 111 1 -- ls -al out) > out
$ cat out
-rw-r--r-- 1 msantos msantos 0 Mar 18 07:32 out
$ ls -al out
-rw-r--r-- 1 msantos msantos 48 Mar 18 07:32 out
```

# OPTIONS

None.

# BUILDING

```
make

# also build libdisablefchmod
make all

# statically linked executable
./musl-make
```

# SEE ALSO

*exec(3)*, *fchmod*(2)
