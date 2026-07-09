/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:53:59 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/09 14:26:33 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_dongle		t_dongle;

typedef struct s_waiter
{
	int						coder_id;
	long					value;
	int						tie_id;
	t_dongle				*first;
	t_dongle				*second;
}							t_waiter;

typedef struct s_heap
{
	t_waiter				*data;
	int						size;
	int						capacity;
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

struct						s_dongle
{
	bool					is_taken;
	long					release_time;
	t_heap					queue;
};

typedef struct s_simulation	t_simulation;

typedef struct s_coder
{
	int						id;
	int						compile_count;
	long					last_compile_time;
	long					wait_value;
	// t_dongle	*holding_dongle;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	pthread_t				thread;
	t_simulation			*sim;
	pthread_mutex_t			time_mutex;
	int						time_mutex_init;
	pthread_mutex_t			compile_mutex;
	int						compile_mutex_init;
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
	int						log_mutex_init;
	int						lock_init;
	pthread_mutex_t			lock;
	pthread_mutex_t			dongle_lock;
	int						dongle_lock_init;
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
int							do_compile_cycle(t_coder *coder, t_simulation *sim);
int							get_isrunning(t_simulation *sim);
int							check_take_dongles(t_coder *coder);
long						get_time(void);
void						log_action(t_simulation *sim, int id, char *msg);
void						*monitor_routine(void *arg);
void						heap_init(t_heap *h, int capacity);
void						heap_push(t_heap *h, t_waiter w);
t_waiter					heap_pop(t_heap *h);
void						bubble_up(t_heap *h, int i);
void						bubble_down(t_heap *h, int i);
int							waiter_less(t_waiter *a, t_waiter *b);
void						swap(t_waiter *a, t_waiter *b);
long						get_waiter_value(t_coder *c);
void						release_dongle(t_simulation *sim, t_dongle *d);
int							take_dongles(t_coder *c);
void						remove_waiter(t_dongle *d, int id);
int							coder_finished(t_coder *c);
void						smart_sleep(long time, t_simulation *sim);
void						register_waiter(t_coder *c);
t_dongle					*first_of(t_coder *c);
t_dongle					*second_of(t_coder *c);
int							dongle_ready(t_dongle *d);
int							try_take_both(t_coder *c);

#endif