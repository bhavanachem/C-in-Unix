# Operating Systems Projects (C)

Systems programming projects from my Operating Systems course at the University of Texas at Dallas, written in C on Linux. They cover file I/O, CPU scheduling, and multithreaded concurrency with POSIX threads, mutexes, and semaphores.

| Project | Topic | Key concepts |
|---|---|---|
| [Project 0: Multi-file line printer](#project-0-multi-file-line-printer) | File I/O | `fopen`/`fgets`, argument parsing, error handling, exit codes |
| [Project 1: MLFQ scheduler simulator](#project-1-mlfq-scheduler-simulator) | CPU scheduling | Multi-level feedback queues, round-robin, priority boost, response and turnaround time |
| [Project 2: Mentoring center concurrency](#project-2-mentoring-center-concurrency-csmc) | Concurrency | POSIX threads, mutexes, semaphores, priority queues, producer–consumer synchronization |

All projects compile cleanly with `-Wall -Werror`.

---

## Project 0: Multi-file line printer

A command-line utility, similar to `head`, that prints the first *N* lines of one or more files.

**Usage**
```bash
gcc proj0.c -o proj0 -Wall -Werror
./proj0 <file1> <max_lines1> [<file2> <max_lines2> ...]
```

**Example**
```bash
./proj0 notes.txt 3 todo.txt 2
```

**What it does**
- Reads each file line by line with `fgets` and prints up to `max_lines` lines exactly as written.
- Separates each file's output with a blank line.
- Adds a newline when a file doesn't end with one, so output stays aligned and the shell prompt starts on a new line.
- Validates input: reports a missing or negative `max_lines` and files that can't be opened on `stderr`, keeps processing the remaining files, and returns a nonzero exit code if any error occurred.

---

## Project 1: MLFQ scheduler simulator

A simulator of the **Multi-Level Feedback Queue** CPU scheduler (OSTEP, Ch. 8), which balances fast response time for short jobs with fairness for long-running ones.

**Usage**
```bash
gcc proj1.c -o proj1 -Wall -Werror
./proj1 T q S1 M1 S2 M2 ... Sq Mq P1a P1T P2a P2T ... Pna PnT
```

| Argument | Meaning |
|---|---|
| `T` | Priority boost period: every `T` time units, all processes move back to the top queue |
| `q` | Number of queues (1–9); Q1 has the highest priority |
| `Sk`, `Mk` | Time slice and maximum time allotment for queue *k* |
| `Pia`, `PiT` | Arrival time and total CPU time needed for process *i* (up to 26 processes, labeled A–Z) |

**Scheduling rules implemented**
- New processes enter the highest-priority queue (Q1). Ties in arrival go to the lower-numbered process.
- Only the highest non-empty queue runs, round-robin with that queue's time slice.
- A newly arriving process preempts a lower-priority process immediately.
- A process that uses up its queue's time allotment is demoted one level.
- Every `T` time units, all processes are boosted back to Q1 (in queue order) to prevent starvation.
- Edge cases follow the spec's ordering: arrivals, demotions and re-queues happen before priority boosts at the same time step.

**Output**
- Each time unit: the queue level and process that ran (e.g. `3 A`).
- On completion: each process's response time and turnaround time (e.g. `A 0 10`).
- At the end: finish time of the last process, plus average response and turnaround time.

---

## Project 2: Mentoring center concurrency (csmc)

A multithreaded simulation of a tutoring center, a variation of the classic **sleeping barber** synchronization problem. Students, tutors and a coordinator each run as their own POSIX thread and coordinate through mutexes and semaphores.

**Usage**
```bash
gcc csmc.c -o csmc -Wall -Werror -pthread
./csmc <#students> <#tutors> <#chairs> <#help>
./csmc 10 3 4 5
```

**How it works**
- **Students** program (sleep), then visit the center. If a waiting-room chair is free they take it and notify the coordinator; if not, they leave and try again later. Each student exits after receiving help the required number of times.
- **The coordinator** places each arriving student into a priority queue (fewer past visits = higher priority, ties broken by arrival order) and signals an idle tutor.
- **Tutors** wait for the coordinator's signal, take the highest-priority waiting student, tutor them, and wait for the next one.
- When every student finishes, all threads shut down cleanly and all dynamically allocated memory is freed.

**Concurrency concepts**
- Semaphores signal between threads (student → coordinator → tutor → student) without busy-waiting.
- Mutexes protect shared state: free chair count, the waiting queue, and session counters.
- Two queues coordinate the hand-off: an arrival queue for the coordinator and a priority queue for tutors.
- Data structures are sized at runtime from the input, and the program handles thousands of threads.

---

## Build requirements
- Linux or macOS with `gcc`
- POSIX threads (`-pthread`) for Project 2

## Author
**Bhavana Chemuturi** · Computer Science, University of Texas at Dallas
[LinkedIn](https://linkedin.com/in/bhavana-chemuturi) · [GitHub](https://github.com/bhavanachem)
