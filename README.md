# mygrep — Flow Diagram

```mermaid
flowchart TD
    subgraph Build["Build (Makefile)"]
        MC[main.c] -->|clang -c| MO[main.o]
        GC[grep.c] -->|clang -c| GO[grep.o]
        FC[fileio.c] -->|clang -c| FO[fileio.o]
        MO & GO & FO -->|link| EXE[mygrep]
    end

    subgraph Header["grep.h — shared types & declarations"]
        GrepOptions["GrepOptions struct\n• pattern\n• line_numbers\n• ignore_cases\n• count_only"]
        Decls["Declarations\n• grep_match_line()\n• grep_search_file()\n• process_files()"]
    end

    subgraph Runtime["Runtime execution"]
        CLI["CLI args\n./mygrep [-n] [-i] [-c] pattern [file...]"]
        CLI --> main["main()\nmain.c\n\nParse flags → build GrepOptions\nExtract pattern\nCall process_files()"]

        main -->|"argc, argv, file_start, &opts"| process_files["process_files()\nfileio.c\n\nNo files → read stdin\nElse → fopen() each file"]

        process_files -->|"FILE*, filename, opts"| grep_search_file["grep_search_file()\ngrep.c\n\nRead line-by-line (1KB buffer)\nCall grep_match_line() per line\nTrack line_number & match_count"]

        grep_search_file -->|"line, opts"| grep_match_line["grep_match_line()\ngrep.c\n\nignore_cases → strcasestr()\nelse → strstr()"]

        grep_match_line -->|"match: 1 / no match: 0"| grep_search_file

        grep_search_file -->|"-c flag"| count_out["stdout: filename:count"]
        grep_search_file -->|"match + no -c"| line_out["stdout:\n-n → filename:linenum:line\ndefault → filename:line"]
    end

    Header -.->|included by| MC & GC & FC
```

# mygrep

A handwritten implementation of the classic Unix `grep` tool, written in C as a learning project.

## What it does

`mygrep` searches for a text pattern inside one or more files (or standard input) and prints every line that contains a match — just like the real `grep`. It supports case-insensitive search, line number display, and match counting.

## Project structure

```
mygrep/
├── main.c      — entry point, argument parsing, flag handling
├── grep.c      — core matching logic (grep_match_line, grep_search_file)
├── fileio.c    — opens files and feeds them to the search engine
├── grep.h      — shared types and function declarations
└── Makefile    — build system
```

## Build

Requires `clang` and `make`.

```bash
make
```

This produces the `mygrep` binary in the current directory. The build includes AddressSanitizer (`-fsanitize=address`) so memory errors are caught at runtime during development.

To clean up compiled artifacts:

```bash
make clean
```

## Usage

```
./mygrep [-n] [-i] [-c] pattern [file ...]
```

| Flag | Meaning |
|------|---------|
| `-n` | Print the line number before each matching line |
| `-i` | Case-insensitive search (ignores upper/lower distinction) |
| `-c` | Count only — print the number of matching lines per file, not the lines themselves |

Flags can be combined in any order, but must come before the pattern.

### Search a single file

```bash
./mygrep hello test.txt
```

```
test.txt:hello world
test.txt:hello again
```

### Show line numbers

```bash
./mygrep -n hello test.txt
```

```
test.txt:1:hello world
test.txt:3:hello again
```

### Case-insensitive search

```bash
./mygrep -i HELLO test.txt
```

```
test.txt:hello world
test.txt:hello again
```

### Count matches only

```bash
./mygrep -c hello test.txt
```

```
test.txt:2
```

### Combine flags

```bash
./mygrep -n -i -c HELLO test.txt
```

```
test.txt:2
```

### Search multiple files

```bash
./mygrep -n hello test.txt other.txt
```

### Read from standard input (stdin)

If no file is given, `mygrep` reads from stdin:

```bash
echo "hello world" | ./mygrep hello
```

```
stdin:hello world
```

Or interactively:

```bash
./mygrep hello
hello world        ← you type this, then press Enter
stdin:hello world  ← mygrep prints the match
```

Press `Ctrl+D` to signal end of input.

## Limitations

- Lines longer than 1023 characters are truncated (fixed 1 KB read buffer).
- No regular expression support — pattern matching is plain substring search only.
- Flags cannot be combined into a single argument (e.g. `-ni` does not work; use `-n -i` instead).
