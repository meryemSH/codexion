*This project has been created as part of the 42 curriculum by mseghrou.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. A group of coders sit around a shared Quantum Compiler, each requiring two USB dongles (one left, one right) to compile their quantum code. After compiling, a coder debugs, then refactors, then attempts to compile again. If a coder goes too long without compiling, they burn out and the simulation ends.

The project is implemented in C using POSIX threads and mutexes. It explores resource synchronization, deadlock prevention, starvation avoidance, and fair scheduling — all under strict timing constraints.

Key features:
- Each coder is a POSIX thread competing for shared dongles
- Dongles have a cooldown period after release
- Two scheduling policies: **FIFO** (first-come, first-served) and **EDF** (Earliest Deadline First)
- A dedicated monitor thread detects burnout within 10 ms
- A custom min-heap priority queue handles fair dongle arbitration

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` binary compiled with `-Wall -Wextra -Werror -pthread`.

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. The scheduler must be exactly `fifo` or `edf` (case-insensitive).

### Examples

```bash
# 5 coders, 1500ms burnout, 200ms compile, 100ms debug, 100ms refactor, 5 compiles required, 50ms cooldown, FIFO scheduler
./codexion 5 1500 200 100 100 5 50 fifo

# Same but with EDF scheduler and a cooldown
./codexion 5 1500 200 100 100 5 50 edf

# Single coder (one dongle, infinite simulation if compiles_required = 0)
./codexion 1 500 100 100 100 0 60 fifo
```

### Cleanup

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # full rebuild
```

## Blocking Cases Handled

**Deadlock prevention (Coffman's conditions):**
Deadlock in this problem arises when every coder holds one dongle and waits for the other. This is prevented by enforcing a consistent global ordering on dongle acquisition: a coder always picks up the dongle with the lower memory address first (`first_dongle` / `second_dongle` in `routine_utils.c`). This breaks the circular-wait condition — one of Coffman's four necessary conditions for deadlock — making deadlock impossible.

**Starvation prevention:**
Without a fair scheduling policy, a coder could be indefinitely bypassed in favor of others. This is addressed by the priority queue inside each dongle: once a coder enqueues itself as a waiter, it will only be granted the dongle when it reaches the front of the queue. Under FIFO, requests are served in arrival order. Under EDF, the coder with the nearest burnout deadline is served first, prioritizing those most at risk. The tie-breaker (coder ID) ensures a fully deterministic ordering when deadlines are equal.

**Cooldown handling:**
Each dongle tracks its `release_time`. After a release, `try_take_dongle` checks `get_time() >= d->release_time` before granting access, enforcing the cooldown without any additional thread sleeping.

**Precise burnout detection:**
The monitor thread runs a tight polling loop, checking each coder's `last_compile_time` against the current time under `time_mutex`. If the elapsed time reaches `time_to_burnout`, it immediately sets `is_running = 0`, logs the burnout, and exits. The polling interval is effectively bounded by `usleep(500)` in `smart_sleep`, ensuring burnout is detected and logged within well under 10 ms.

**Log serialization:**
All calls to `log_action` acquire `sim->log_mutex` before printing, so no two messages can interleave on stdout.

## Thread Synchronization Mechanisms

**`pthread_mutex_t` — the primary synchronization primitive:**

| Mutex | Protects |
|-------|----------|
| `sim->lock` | `sim->is_running` (global stop flag) |
| `sim->log_mutex` | stdout output — prevents interleaved log lines |
| `dongle->mutex` | dongle state: `is_taken`, `release_time`, and the waiter queue |
| `coder->time_mutex` | `coder->last_compile_time` — written by the coder thread, read by the monitor |
| `coder->compile_mutex` | `coder->compile_count` — written by the coder, read by the monitor |

**Preventing race conditions:**

- `last_compile_time` is always updated inside `coder->time_mutex` (in `do_compile_cycle`) and read inside the same mutex (in the monitor and in `get_waiter_value` for EDF). There is no window where the monitor can read a partially-updated timestamp.
- The `is_running` flag is always read via `get_isrunning()` and written under `sim->lock`, so coder threads and the monitor agree on the stopped state without torn reads.
- The dongle queue (heap) is only ever modified while `dongle->mutex` is held, so concurrent `try_take_dongle` calls from different coder threads cannot corrupt the heap structure.

**Thread-safe communication between coders and the monitor:**

Coders do not signal the monitor directly. Instead, the monitor polls shared state that is protected by mutexes. When the monitor decides the simulation must stop, it sets `is_running = 0` under `sim->lock` and logs the burnout under `sim->log_mutex`. Coder threads check `get_isrunning()` at each iteration of their main loop and after every `smart_sleep`, so they observe the flag change promptly and exit cleanly.

**No `pthread_cond_t` is used:** waiting coders do not block on a condition variable. Instead, `check_take_dongles` returns 0 and the coder loop calls `smart_sleep(1000, sim)` — a 1 ms interruptible sleep that also monitors `is_running` — before retrying. This keeps the design simple and avoids the complexity of spurious wakeups, while still allowing timely termination.

## Resources

- [The POSIX Threads Programming guide (LLNL)](https://hpc-tutorials.llnl.gov/posix/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Binary Heap — Wikipedia](https://en.wikipedia.org/wiki/Binary_heap)
- [`pthread_mutex_t` man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`gettimeofday` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

**AI usage:**
AI usage:
AI was used as a supplementary learning and occasionally to clarify concepts