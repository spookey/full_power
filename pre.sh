#!/usr/bin/env bash

ROOT_DIR=$(cd "$(dirname "$0")" || exit 1; pwd)

CRUMBS="$ROOT_DIR/lib/serve"
SOURCE=${SOURCE-"$CRUMBS/raw"}
TARGET=${TARGET-"$CRUMBS/inc"}
DEFINE=${DEFINE-"_INCL_STRING"}
PRESET=('basic.html' 'funct.js' 'plugs.html' 'style.css')
PREFIX=${PREFIX-"_"}


msg() { echo -e "$(basename "$0")" "|" "$@"; }

FORCED=false
while getopts ":f" OPTION; do
    case $OPTION in
        f) FORCED=true;;
        *) msg "unknown option";;
    esac
done

if [[ ! -d "$TARGET" ]]; then
    mkdir -p "$TARGET"
    msg "created target path" "$TARGET"
fi

for NAME in "${PRESET[@]}"; do
    src="$SOURCE/$NAME"
    tgt="$TARGET/$PREFIX$NAME"
    if [[ $FORCED == false && -e $tgt ]]; then
        msg "skipping" "$NAME" "already present in" "$(basename "$TARGET")"
        continue
    fi
    if [[ ! -e $src ]]; then
        msg "file" "$NAME" "not present in" "$(basename "$SOURCE")"
        continue
    fi
    printf '%s(\n%s\n)\n' "$DEFINE" "$(<"$src")" > "$tgt"
    msg "file" "$NAME" "done"
done

exit 0
