#!/usr/bin/env bash
# init-project.sh — Rename all frame placeholders to your project's names.
#
# Usage:
#   ./scripts/init-project.sh --name "My Cool App" [--prefix mca] [--apply]
#
# By default runs in dry-run mode, showing a diff of proposed changes.
# Pass --apply to execute the replacements.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

NAME=""
PREFIX=""
APPLY=false

usage() {
    cat <<EOF
Usage: $0 --name "Project Name" [--prefix abc] [--apply]

  --name     Human-readable project name (required)
  --prefix   Short identifier prefix (derived from name if omitted)
  --apply    Execute changes (dry-run by default)
  --help     Show this help
EOF
    exit 0
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --name)   NAME="$2";   shift 2 ;;
        --prefix) PREFIX="$2"; shift 2 ;;
        --apply)  APPLY=true;  shift   ;;
        --help)   usage ;;
        *)        echo "Unknown option: $1"; usage ;;
    esac
done

if [[ -z "$NAME" ]]; then
    echo "Error: --name is required"
    usage
fi

# Derive snake_case project name: lowercase, spaces to underscores
SNAKE_NAME=$(echo "$NAME" | tr '[:upper:]' '[:lower:]' | tr ' ' '_')

# Derive prefix from initials if not provided
if [[ -z "$PREFIX" ]]; then
    PREFIX=$(echo "$NAME" | tr '[:upper:]' '[:lower:]' | awk '{for(i=1;i<=NF;i++) printf substr($i,1,1)}')
fi

PREFIX_UPPER=$(echo "$PREFIX" | tr '[:lower:]' '[:upper:]')
PREFIX_LOWER=$(echo "$PREFIX" | tr '[:upper:]' '[:lower:]')

# Derive PascalCase for class names: "My Cool App" -> "MyCoolApp"
PASCAL_NAME=$(echo "$NAME" | sed -E 's/(^| )(.)/\U\2/g' | tr -d ' ')

echo "Project setup:"
echo "  Name:          $NAME"
echo "  Snake name:    $SNAKE_NAME"
echo "  Prefix:        $PREFIX_LOWER"
echo "  Prefix upper:  $PREFIX_UPPER"
echo "  PascalCase:    $PASCAL_NAME"
echo ""

# Files to process (tracked files only, excluding this script and binary/generated content)
TRACKED_FILES=$(cd "$REPO_ROOT" && git ls-files -- \
    '*.cpp' '*.h' '*.cmake' '*.txt' '*.md' '*.yml' '*.yaml' '*.json' '*.in' '*.service' '*.desktop' \
    | grep -v 'scripts/init-project.sh' \
    | grep -v 'docs/superpowers/' \
    || true)

do_replace() {
    local file="$1"
    local filepath="$REPO_ROOT/$file"

    [[ -f "$filepath" ]] || return 0

    # Order matters: longest/most-specific patterns first
    sed -i \
        -e "s/cpp_agentic_development_frame/${SNAKE_NAME}/g" \
        -e "s/ProjectCoreTest/${PASCAL_NAME}CoreTest/g" \
        -e "s/ProjectCore/${PASCAL_NAME}Core/g" \
        -e "s/namespace frame::test/namespace ${PREFIX_LOWER}::test/g" \
        -e "s/namespace frame/namespace ${PREFIX_LOWER}/g" \
        -e "s/projectcore/${PREFIX_LOWER}core/g" \
        -e "s/project_core/${PREFIX_LOWER}_core/g" \
        -e "s/frame_cli/${PREFIX_LOWER}_cli/g" \
        -e "s/frame::/${PREFIX_LOWER}::/g" \
        -e "s/FRAME_/${PREFIX_UPPER}_/g" \
        -e "s/frame_enable_/${PREFIX_LOWER}_enable_/g" \
        -e "s/frame_add_test/${PREFIX_LOWER}_add_test/g" \
        -e "s/frame_bench/${PREFIX_LOWER}_bench/g" \
        -e "s/frame_test/${PREFIX_LOWER}_test/g" \
        "$filepath"
}

do_rename_file() {
    local old_path="$REPO_ROOT/$1"
    [[ -f "$old_path" ]] || return 0
    local dir=$(dirname "$old_path")
    local old_name=$(basename "$old_path")
    local new_name=$(echo "$old_name" \
        | sed "s/projectcore/${PREFIX_LOWER}core/g" \
        | sed "s/frame_test/${PREFIX_LOWER}_test/g" \
        | sed "s/frame_bench/${PREFIX_LOWER}_bench/g")

    if [[ "$old_name" != "$new_name" ]]; then
        if $APPLY; then
            git -C "$REPO_ROOT" mv "$old_path" "$dir/$new_name"
        else
            echo "  rename: $1 -> $(dirname "$1")/$new_name"
        fi
    fi
}

if $APPLY; then
    echo "Applying changes..."
    echo ""

    # Content replacements
    for file in $TRACKED_FILES; do
        do_replace "$file"
    done

    # File renames (after content replacement)
    for file in $TRACKED_FILES; do
        do_rename_file "$file"
    done

    echo "Done. Review changes with: git diff"
else
    echo "=== DRY RUN ==="
    echo ""

    # Show what file renames would happen
    echo "File renames:"
    for file in $TRACKED_FILES; do
        do_rename_file "$file"
    done
    echo ""

    # Create a temporary copy to show diff
    TMPDIR=$(mktemp -d)
    trap 'rm -rf "$TMPDIR"' EXIT

    for file in $TRACKED_FILES; do
        [[ -f "$REPO_ROOT/$file" ]] || continue
        mkdir -p "$TMPDIR/$(dirname "$file")"
        cp "$REPO_ROOT/$file" "$TMPDIR/$file"
    done

    # Apply replacements to temp copies
    for file in $TRACKED_FILES; do
        [[ -f "$TMPDIR/$file" ]] || continue
        sed -i \
            -e "s/cpp_agentic_development_frame/${SNAKE_NAME}/g" \
            -e "s/ProjectCoreTest/${PASCAL_NAME}CoreTest/g" \
            -e "s/ProjectCore/${PASCAL_NAME}Core/g" \
            -e "s/namespace frame::test/namespace ${PREFIX_LOWER}::test/g" \
            -e "s/namespace frame/namespace ${PREFIX_LOWER}/g" \
            -e "s/projectcore/${PREFIX_LOWER}core/g" \
            -e "s/project_core/${PREFIX_LOWER}_core/g" \
            -e "s/frame_cli/${PREFIX_LOWER}_cli/g" \
            -e "s/frame::/${PREFIX_LOWER}::/g" \
            -e "s/FRAME_/${PREFIX_UPPER}_/g" \
            -e "s/frame_enable_/${PREFIX_LOWER}_enable_/g" \
            -e "s/frame_add_test/${PREFIX_LOWER}_add_test/g" \
            -e "s/frame_bench/${PREFIX_LOWER}_bench/g" \
            -e "s/frame_test/${PREFIX_LOWER}_test/g" \
            "$TMPDIR/$file"
    done

    echo "Content changes:"
    for file in $TRACKED_FILES; do
        [[ -f "$TMPDIR/$file" ]] || continue
        diff -u "$REPO_ROOT/$file" "$TMPDIR/$file" --label "a/$file" --label "b/$file" 2>/dev/null || true
    done

    echo ""
    echo "Run with --apply to execute these changes."
fi
