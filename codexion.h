#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_args
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			scheduler[5];
}					t_args;

typedef struct s_dongle
{
	int				is_taken;
	long			release_time;
	pthread_mutex_t	mutex;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile_time;
	int				is_alive;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_t		thread;
}					t_coder;

typedef struct s_simulation
{
	t_args			args;
	t_coder			*coders;
	t_dongle		*dongles;
	int				is_running;
	pthread_mutex_t log_mutex;
}					t_simulation;

int	init_simulation(t_simulation *sim);

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