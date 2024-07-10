# DuckDB+C Starter Template

## Usage

All chores are put in convenient bash files in the script directory.

### Preparation

This needs to be run the first time only.

```bash
./scripts/install.sh
```

This will download, verify, and extract DuckDB v1.0.0 in the
thirdparty directory. If the `LD_LIBRARY_PATH` is not set, the script
will set it for you.

### Compilation

To compile the C code, run:

```bash
./scripts/compile.sh
```

There's no Makefile for this project (intentionally).

### Running

To run the compiled code, run:

```bash
./bin/main
```

## Modification

Update the script files to suit your needs. The `main.c` file is a
moderately involved example of how to use DuckDB in a C program.

## License

This project is licensed under the MIT License. See the LICENSE file
for details.

## Contributing

Contributions are welcome! Create issues for bugs or feature requests
and submit pull requests for improvements (corresponsing to the issues).
