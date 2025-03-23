# Multithreaded System Runner

This project is a multithreaded C program that initializes a custom system context from a configuration file, spawns multiple threads to perform defined tasks, and manages the lifecycle of the system.

## Features
- Dynamic system initialization from a configuration file
- Thread management (creation, joining, cleanup)
- Safe memory allocation utilities
- Configurable system behavior
- Custom thread functions
- Internal queue structure for thread communication (if applicable)

## File Structure
```
include/             # Header files
  config.h
  queue.h
  safe_alloc.h
  system.h
  thread_funcs.h
  types.h

src/                 # Source files
  config.c
  main.c
  queue.c
  safe_alloc.c
  thread_funcs.c

config.txt           # Example configuration file
Makefile             # For building the project
README.md            # Project documentation
.gitignore           # Git ignore rules
app                  # Output binary (after build)
```

## Building the Project
Make sure you have `gcc` or another C compiler installed, then run:

```sh
make
```

This will compile the source files and generate an executable named `app`.

## Running the Program
Run the compiled executable with a configuration file as an argument:

```sh
./app config.txt
```

## Configuration
The system reads settings from `config.txt`. Ensure it contains the expected format for your application. You can define things like:
- Number of threads
- Task types
- Timing constraints

Refer to `config.h` and `config.c` for specific keys and formats supported.

## Thread System
The threading model is defined in `thread_funcs.h` and implemented in `thread_funcs.c`. Threads are created and managed via the `startThreads` and `joinThreads` functions.

## Memory Safety
The `safe_alloc.h` module wraps memory allocation functions to add checks and safety against failures.
