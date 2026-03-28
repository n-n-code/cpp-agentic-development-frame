#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 ]]; then
    printf 'Usage: %s <build-dir>\n' "${0##*/}" >&2
    exit 2
fi

build_dir="$1"

if [[ ! -d "$build_dir" ]]; then
    printf 'Build directory not found: %s\n' "$build_dir" >&2
    exit 2
fi

if ! command -v valgrind >/dev/null 2>&1; then
    printf 'valgrind was not found in PATH.\n' >&2
    exit 2
fi

run_memcheck() {
    local label="$1"
    shift

    printf 'Running Valgrind Memcheck: %s\n' "$label"
    valgrind \
        --tool=memcheck \
        --leak-check=full \
        --show-leak-kinds=definite,possible \
        --errors-for-leak-kinds=definite,possible \
        --track-origins=yes \
        --num-callers=30 \
        --show-error-list=yes \
        --error-exitcode=101 \
        "$@"
}

require_binary() {
    local path="$1"

    if [[ ! -x "$path" ]]; then
        printf 'Expected executable not found: %s\n' "$path" >&2
        exit 2
    fi
}

app_binary="${build_dir}/frame_cli"
project_core_test="${build_dir}/tests/projectcoretest"
cli_smoke_test="${build_dir}/tests/clismoketest"

require_binary "$app_binary"
require_binary "$project_core_test"
require_binary "$cli_smoke_test"

run_memcheck "projectcoretest" "$project_core_test"
run_memcheck "clismoketest" "$cli_smoke_test"
run_memcheck "frame_cli --help" "$app_binary" --help
