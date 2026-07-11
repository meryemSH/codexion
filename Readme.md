# Codexion

> *This project has been created as part of the 42 curriculum by mseghrou.*

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem.

A group of coders share a Quantum Compiler. To compile their quantum code, each coder needs **two USB dongles** (left and right). After compiling, the coder debugs the code, refactors it, and then starts another compilation cycle.

If a coder does not compile again before `time_to_burnout`, they burn out and the simulation immediately stops.

The project is written in **C** using **POSIX threads** and **mutexes**, focusing on:

- Thread synchronization
- Resource sharing
- Deadlock prevention
- Starvation avoidance
- Fair scheduling
- Atomic resource acquisition

Unlike the classical Dining Philosophers solution, Codexion never allows a coder to hold only one dongle. A coder either acquires **both required dongles atomically** or acquires none.

---

# Features

- One POSIX thread per coder
- One monitor thread
- Shared USB dongles
- Dongle cooldown support
- FIFO scheduler
- EDF (Earliest Deadline First) scheduler
- Custom binary min-heap priority queues
- Atomic acquisition of both dongles
- Convoy-effect prevention
- Thread-safe logging
- Burnout detection within 10 ms

---

# Compilation

```bash
make
```

This generates:

```text
./codexion
```

Compilation flags:

```text
-Wall -Wextra -Werror -pthread
```

---

# Usage

```bash
./codexion \
number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

All arguments are mandatory.

The scheduler must be:

```text
fifo
```

or

```text
edf
```

(case-insensitive).

---

# Examples

FIFO

```bash
./codexion 5 1500 200 100 100 5 50 fifo
```

EDF

```bash
./codexion 5 1500 200 100 100 5 50 edf
```

Single coder

```bash
./codexion 1 500 100 100 100 1 60 fifo
```

---

# Cleanup

```bash
make clean
```

Remove object files.

```bash
make fclean
```

Remove objects and executable.

```bash
make re
```

Full rebuild.

---

# Scheduling Policies

## FIFO

FIFO (First In, First Out) serves coders in the order they request the dongles.

Each waiter receives a timestamp when entering the queue.

The earliest request always has the highest priority.

---

## EDF

EDF (Earliest Deadline First) gives priority to the coder that is closest to burning out.

The priority value is computed from:

```text
last_compile_time + time_to_burnout
```

The smallest deadline has the highest priority.

If two coders have the same deadline, the coder ID is used as a deterministic tie-breaker.

---

# Deadlock Prevention

Codexion prevents deadlock by **never allowing partial acquisition**.

Before attempting to compile, a coder:

1. Registers itself in the waiting queue of both required dongles.
2. Waits until both dongles are simultaneously available.
3. Acquires both dongles atomically while holding a single global mutex.

Therefore, a coder never owns one dongle while waiting for the second one.

Since the circular-wait condition never exists, deadlock is impossible.

---

# Starvation Prevention

Each dongle maintains a priority queue of waiting coders.

When multiple coders compete for the same dongle, the scheduler selects the highest-priority waiter.

Priority depends on:

- FIFO order
- EDF deadline

Every waiter is eventually served according to the selected scheduling policy.

---

# Convoy Effect Prevention

A higher-priority waiter does **not** automatically block every other coder.

Before preventing another coder from taking the dongles, the scheduler verifies that the higher-priority waiter can immediately acquire **both** of its required dongles.

If that waiter is itself blocked by another unavailable dongle, it is temporarily ignored.

This prevents unnecessary serialization of the simulation and allows runnable coders to continue making progress.

---

# Atomic Dongle Acquisition

Dongles are never acquired independently.

The scheduler first checks:

- both dongles are available
- both cooldowns have expired
- no runnable higher-priority waiter exists

Only then are both dongles marked as taken.

Otherwise, neither dongle is acquired.

This guarantees atomic resource acquisition.

---

# Dongle Cooldown

Each dongle stores:

```text
release_time
```

After a release, the dongle cannot be reused until:

```text
current_time >= release_time
```

The cooldown is enforced without additional sleeping threads.

---

# Burnout Detection

A dedicated monitor thread continuously checks every coder.

For each coder:

```text
current_time - last_compile_time
```

is compared against:

```text
time_to_burnout
```

If the timeout is reached:

- the burnout is logged
- the simulation stops
- every thread exits cleanly

Detection occurs within the required 10 ms limit.

---

# Thread Synchronization

The project uses POSIX mutexes.

| Mutex | Protects |
|--------|----------|
| `sim->lock` | `is_running` |
| `sim->log_mutex` | stdout logging |
| `sim->dongle_lock` | all dongles, waiter queues, acquisition and release |
| `coder->time_mutex` | `last_compile_time` |
| `coder->compile_mutex` | `compile_count` |

---

# Race Condition Prevention

Shared data is always accessed while holding the appropriate mutex.

Examples:

- `is_running` is protected by `sim->lock`
- `last_compile_time` is protected by `time_mutex`
- `compile_count` is protected by `compile_mutex`
- the complete dongle subsystem is protected by `sim->dongle_lock`

This prevents concurrent modifications of:

- waiter queues
- dongle ownership
- cooldown timers
- scheduling decisions

---

# Thread Communication

Coders never communicate directly with one another.

Instead:

- coder threads update shared state
- the monitor periodically checks that state
- if burnout occurs, the monitor sets `is_running` to `0`
- every coder observes the flag and exits safely

---

# Waiting Strategy

The project intentionally avoids using `pthread_cond_t`.

When a coder cannot acquire both dongles:

- it retries later
- sleeps for 1 ms using an interruptible sleep
- checks `is_running`
- retries acquisition

This keeps the implementation simple while maintaining responsive shutdown.

---

# Logging

All logging passes through:

```c
log_action(...)
```

which is protected by:

```text
sim->log_mutex
```

This guarantees that log lines never interleave on stdout.

---

# Data Structures

The scheduler relies on a custom binary min-heap.

Each waiter stores:

- coder id
- scheduling priority
- tie-break id
- pointer to first required dongle
- pointer to second required dongle

The heap provides efficient priority ordering for both FIFO and EDF.

---

# Testing

The project was tested using:

- Normal execution
- Stress tests
- AddressSanitizer
- Valgrind Memcheck
- Valgrind Helgrind
- Valgrind DRD

The implementation reports:

- no memory leaks
- no race conditions
- no synchronization errors

---

# Resources

- POSIX Threads Programming Guide (LLNL)
- Dining Philosophers Problem — Wikipedia
- Earliest Deadline First Scheduling — Wikipedia
- Binary Heap — Wikipedia
- pthread mutex documentation
- gettimeofday documentation

---

# AI Usage

AI was used only as a supplementary learning tool to discuss concurrency concepts, scheduling strategies, synchronization mechanisms, and documentation improvements.

All design decisions, implementation, debugging, testing, and final validation were performed by me.