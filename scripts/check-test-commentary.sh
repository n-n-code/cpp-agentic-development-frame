#!/usr/bin/env bash

set -euo pipefail

repo_root="${1:-$(pwd)}"
status=0

while IFS= read -r test_file; do
    awk '
        function reset_state() {
            saw_what = 0
            saw_how = 0
            saw_why = 0
            in_function = 0
            test_line = 0
        }

        /^[[:space:]]*void[[:space:]]+[A-Za-z0-9_]+::[A-Za-z0-9_]+\(/ {
            reset_state()
            in_function = 1
            test_line = NR
            next
        }

        in_function {
            if ($0 ~ /^[[:space:]]*\/\/[[:space:]]*WHAT:/) {
                saw_what = 1
                next
            }
            if ($0 ~ /^[[:space:]]*\/\/[[:space:]]*HOW:/) {
                saw_how = 1
                next
            }
            if ($0 ~ /^[[:space:]]*\/\/[[:space:]]*WHY:/) {
                saw_why = 1
                next
            }
            if ($0 ~ /^[[:space:]]*$/ || $0 ~ /^[[:space:]]*{[[:space:]]*$/ || $0 ~ /^[[:space:]]*\/\/.*$/) {
                next
            }
            if (!(saw_what && saw_how && saw_why)) {
                printf("%s:%d: missing WHAT/HOW/WHY block near test body\n", FILENAME, test_line)
                missing = 1
            }
            reset_state()
        }

        END {
            if (in_function && !(saw_what && saw_how && saw_why)) {
                printf("%s:%d: missing WHAT/HOW/WHY block near test body\n", FILENAME, test_line)
                missing = 1
            }
            if (missing) {
                exit 1
            }
        }
    ' "$test_file" || status=1
done < <(find "$repo_root/tests" -maxdepth 1 -name '*test.cpp' | sort)

exit "$status"
