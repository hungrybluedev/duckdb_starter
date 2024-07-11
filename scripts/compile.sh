#!/bin/bash

CC="gcc"
CFLAGS="-Wall -Wextra -Werror -O3 -g"
INCLUDES="-Ithirdparty/libduckdb -Iinclude"

main() {
  # Check if we are in the root directory of the project
  if [ ! -f "scripts/compile.sh" ]; then
    echo "Please run this script from the root directory of the project."
    exit 1
  fi

  rm -rf bin db
  mkdir bin db

  # Compile all the c files in src to o files in bin
  for file in src/*.c; do
    filename=$(basename -- "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"
    $CC $INCLUDES $CFLAGS -c $file -o bin/$filename.o
  done

  # Link all the o files in bin to an executable in bin
  $CC bin/*.o thirdparty/libduckdb/libduckdb.so -o bin/main
}

main