#!/bin/bash

sleep 1

APP="$1"
DIR="$(dirname "$APP")"

cd "$DIR" || exit 1

nohup "$APP" >/dev/null 2>&1 &
