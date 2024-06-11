#!/usr/bin/env bats

@test "fchmodexec: change file permissions" {
  umask 0077
  TMPFILE="$(mktemp)"
  run stat --format="%a" "$TMPFILE"
  [ "$output" = "600" ]

  run sh -c "(fchmodexec 764 1 -- true) > $TMPFILE"
  [ "$status" -eq 0 ]

  run stat --format="%a" "$TMPFILE"
  cat << EOF
expected: 764
got: $output
EOF
  [ "$output" = "764" ]

  rm "$TMPFILE"
}

@test "fchmodexec: ignore bad fd" {
  umask 0077
  TMPFILE="$(mktemp)"
  run stat --format="%a" "$TMPFILE"
  [ "$output" = "600" ]

  run sh -c "(fchmodexec 764 111 112 99999999 1 -- true) > $TMPFILE"
  [ "$status" -eq 0 ]

  run stat --format="%a" "$TMPFILE"
  cat << EOF
expected: 764
got: $output
EOF
  [ "$output" = "764" ]

  rm "$TMPFILE"
}

@test "fchmodexec: validate arguments before fchmod" {
  umask 0077
  TMPFILE="$(mktemp)"
  run stat --format="%a" "$TMPFILE"
  [ "$output" = "600" ]

  run sh -c "(fchmodexec 310 1 true) > $TMPFILE"
  [ "$status" -ne 0 ]

  run stat --format="%a" "$TMPFILE"
  cat << EOF
expected: 600
got: $output
EOF
  [ "$output" = "600" ]

  rm "$TMPFILE"
}

@test "libdisablefchmod: disable calls to fchmod() on stdin/stdout" {
  umask 0077
  TMPFILE="$(mktemp)"
  run stat --format="%a" "$TMPFILE"
  [ "$output" = "600" ]

  run sh -c "(LD_PRELOAD=libdisablefchmod.so fchmodexec 111 1 -- true > $TMPFILE)"
  [ "$status" -eq 0 ]

  run stat --format="%a" "$TMPFILE"
  cat << EOF
expected: $output
got: 600
EOF
  [ "$output" = "600" ]

  rm "$TMPFILE"
}
