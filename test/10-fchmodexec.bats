#!/usr/bin/env bats

@test "fchmodexec: change file permissions" {
  umask 0077
  TMPFILE="$(mktemp)"
  run stat --format="%a" "$TMPFILE"
  [ "$output" = "600" ]

  run sh -c "(fchmodexec 764 1 true) > $TMPFILE"
  [ "$status" -ne 0 ]

  run stat --format="%a" "$TMPFILE"
  cat << EOF
expected: 764
got: $output
EOF
  [ "$output" = "764" ]

  rm "$TMPFILE"
}
