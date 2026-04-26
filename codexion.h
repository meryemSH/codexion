#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAX_CODERS 256

typedef struct s_waiter
{
	int coder_id;  // which coder is waiting (ex: coder 3)
	long priority; // how urgent (smaller = more urgent)
}							t_waiter;

typedef struct s_heap
{
	t_waiter				data[MAX_CODERS]; // the arary of waiters
	int						size; // how many waiters in it
}							t_heap;

typedef struct s_args
{
	int						number_of_coders;
	int						time_to_burnout;
	int						time_to_compile;
	int						time_to_debug;
	int						time_to_refactor;
	int						number_of_compiles_required;
	int						dongle_cooldown;
	char					scheduler[5];
}							t_args;

typedef struct s_dongle
{
	int						is_taken;
	long					release_time;
	pthread_mutex_t			mutex;
	t_heap					queue;
}							t_dongle;

typedef struct s_simulation	t_simulation;

typedef struct s_coder
{
	int						id;
	int						compile_count;
	long					last_compile_time;
	int						is_alive;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	pthread_t				thread;
	t_simulation			*sim;
}							t_coder;

struct						s_simulation
{
	t_args					args;
	t_coder					*coders;
	t_dongle				*dongles;
	int						is_running;
	long					start_time;
	pthread_t				monitor_thread;
	pthread_mutex_t			log_mutex;
	pthread_mutex_t			lock;
};

int							init_simulation(t_simulation *sim);
int							start_threads(t_simulation *sim);
void						cleanup(t_simulation *sim);
int							is_positive(char *str);
int							check_args(int argc, char **argv);
void						fill_args(char **argv, t_args *args);
void						*routine(void *arg);
int							init_memory(t_simulation *sim);
int							init_mutex(t_simulation *sim);
int							init_dongles(t_simulation *sim);
int							init_coders(t_simulation *sim);
void						join_threads(t_simulation *sim);
long						get_time(void);
void						log_action(t_simulation *sim, int id, char *msg);
void						*monitor(void *arg);
// /////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#endif

// ### Field by field:

// **`id`** — coder number (1 to N)

// **`compile_count`** — how many times this coder compiled. Simulation stops when all coders reach `number_of_compiles_required`.

// **`last_compile_time`** — timestamp of last compile start. Used to check burnout:
// ```
// current_time - last_compile_time > time_to_burnout → burned out! 💀
// ```

// **`is_alive`** — is this coder still alive?
// ```
// 1 = alive
// 0 = burned out
// ```

// **`left_dongle`
// `right_dongle`** — pointers to the two dongles this coder can grab.Remember : dongles are **shared **between neighbors !
// ```
// Coder 1 ←dongle A→ Coder 2 ←dongle B→ Coder 3