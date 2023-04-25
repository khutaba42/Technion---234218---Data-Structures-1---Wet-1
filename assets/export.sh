#!/bin/bash

PROJ_DIR=$(pwd)
SRC_DIR="$PROJ_DIR/src"
LIB_DIR="$PROJ_DIR/lib"
ZIP_DIR="$PROJ_DIR/zipped"
ZIP_FILE="wet_1.zip"

if [ ! -d "$ZIP_DIR" ]; then
  mkdir "$ZIP_DIR"
fi

find "$SRC_DIR" -type f -not -name ".*" -exec cp {} "$ZIP_DIR" \;
find "$LIB_DIR" -type f -not -name ".*" -exec cp {} "$ZIP_DIR" \;

cd "$ZIP_DIR" || exit
zip -r "$ZIP_FILE" .

find "$ZIP_DIR" -type f -not -name "wet_1.zip" -delete
