# Multithreaded News Processing System

This project is a multithreaded news processing system written in C. It simulates a news production pipeline where multiple producer threads generate news articles, a dispatcher thread routes these articles to different news categories (NEWS, SPORTS, and WEATHER), and co-producer threads further process the articles before they are displayed by a screen writer thread.

## Features

- **Multithreading:** Utilizes POSIX threads (pthreads) to concurrently handle news production, dispatching, and screen writing.
- **Thread-Safe Queues:** Implements both unbounded and bounded queues with semaphore-based synchronization to ensure safe concurrent access.
- **Modular Design:** Organized into clear modules for configuration, memory allocation, queue management, and thread functionality to enhance maintainability and scalability.

## Configuration File Format

The application is configured via a text file that specifies the setup for producer queues and the screen queue.

- **Producer Configuration:**  
  Each producer is defined by three integers:
  1. **Producer ID (dummy value):** An identifier for the producer.
  2. **Article Count:** The number of articles the producer will generate.
  3. **Producer Queue Capacity:** The maximum number of items the producer’s bounded queue can hold.

- **Screen Queue Configuration:**  
  After listing all producers, a single integer is provided to specify the capacity of the screen queue.

### Example Config File (`config.txt`)
```
1 30 5

2 25 3

3 16 30

4 18 2

5 12 50

17
```

In this example:
- There are **5 producers**.
  - **Producer 1:** Produces 30 articles; its bounded queue has a capacity of 5.
  - **Producer 2:** Produces 25 articles; its bounded queue has a capacity of 3.
  - **Producer 3:** Produces 16 articles; its bounded queue has a capacity of 30.
  - **Producer 4:** Produces 18 articles; its bounded queue has a capacity of 2.
  - **Producer 5:** Produces 12 articles; its bounded queue has a capacity of 50.
- The last line (`17`) defines the capacity of the **screen queue**.

## Building the Project

You can compile the project using a Makefile or directly with `gcc`. For example:

```bash
gcc -pthread -Iinclude src/*.c -o news_system
```

## Running the Project
Once compiled, run the executable by passing the configuration file as an argument:

```bash
./news_system config.txt
```
The system will simulate the news production process and output the news articles (categorized as NEWS, SPORTS, or WEATHER) to the standard output.
