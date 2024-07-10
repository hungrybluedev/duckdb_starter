#!/bin/bash

download_duckdb() {
  # We'll be using DuckDB v1.0.0 for this project
  # The SHA256 and MD5 hashes of the file are provided for verification
  duck_db_link="https://github.com/duckdb/duckdb/releases/download/v1.0.0/libduckdb-linux-amd64.zip"
  file_hash_sha256="9b8eb65d2ab8f65210029602122194d7b46728a9450cc3d528005e74c7e3b206"
  file_hash_md5="c1f16db93d34ffed0a24d711ce8349f0"
  temporaty_filename="libduckdb-linux-amd64.zip"

  mkdir -p thirdparty
  echo "Downloading ZIP from GitHub...
"
  wget -O thirdparty/${temporaty_filename} $duck_db_link
  echo "ZIP downloaded.
"

  # Check if the file hash matches the expected hash
  if [ "$(sha256sum thirdparty/${temporaty_filename} | awk '{print $1}')" != $file_hash_sha256 ]; then
    echo "The SHA256 hash of the downloaded file does not match the expected hash."
    exit 1
  fi
  if [ "$(md5sum thirdparty/${temporaty_filename} | awk '{print $1}')" != $file_hash_md5 ]; then
    echo "The MD5 hash of the downloaded file does not match the expected hash."
    exit 1
  fi
  echo "File integrety verified!
"

  echo "Unzipping the file...
"
  unzip thirdparty/${temporaty_filename} -d thirdparty/libduckdb
  echo "File unzipped.

Cleaning up...
"
  rm thirdparty/${temporaty_filename}
  echo "Cleanup complete.

DuckDB installed in thirdparty/libduckdb.
"
}

update_library_path() {
  echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/thirdparty/libduckdb" >> ~/.bashrc
  source ~/.bashrc
}

main() {
  # Download the duckdb library if it does not exist
  if [ ! -d "thirdparty/libduckdb" ]; then
    echo "
Downloading duckdb library...

"
    download_duckdb
    echo "
Duckdb library downloaded.

"
  else
    echo "
Duckdb library already exists.

"
  fi

  # Update the library path if we haven't already
  if ! grep -q "LD_LIBRARY_PATH" ~/.bashrc; then
    echo "
Updating library path...

"
    update_library_path
    echo "
Library path updated.

"
  else
    echo "
Library path already updated.

"
  fi
}

main