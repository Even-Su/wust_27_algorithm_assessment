#!/usr/bin/env bash
# TASK2 AI-assisted draft. Review, test on Ubuntu and explain before submitting.
cd "$(dirname "$0")" || exit 1

build() {
    local before='' after=''
    if [[ -f build/app ]]; then
        before=$(stat -c '%y' build/app) || return 1
    fi
    # Let CMake verify all configured dependencies; do not guess from two timestamps.
    cmake -S . -B build >&2 || return $?
    cmake --build build >&2 || return $?
    if [[ ! -f build/app ]]; then
        echo 'Error: expected executable build/app is missing' >&2
        return 1
    fi
    after=$(stat -c '%y' build/app) || return 1
    if [[ -n "$before" && "$before" == "$after" ]]; then
        echo 'Executable is up to date; skipping compilation.'
    fi
}

case "${1:-}" in
    build) build ;;
    run)
        build || exit $?
        shift
        if [[ "${1:-}" == '--' ]]; then shift; fi
        ./build/app "$@"
        ;;
    clean) rm -rf -- build ;;
    help)
        echo 'Usage: ./run.sh {build|run|clean|help}'
        echo 'Pass arguments: ./run.sh run -- arg1 arg2'
        ;;
    *) echo "Unknown command: ${1:-<none>}" >&2; exit 1 ;;
esac
