/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:53:08 by mseghrou          #+#    #+#             */
/*   Updated: 2026/06/29 22:19:31 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(t_simulation *sim)
{
	sim->start_time = get_time();
	if (init_memory(sim))
		return (1);
	if (init_mutex(sim))
	{
		cleanup(sim);
		return (1);
	}
	if (init_dongles(sim))
	{
		cleanup(sim);
		return (1);
	}
	if (init_coders(sim))
	{
		cleanup(sim);
		return (1);
	}
	return (0);
}

int	init_mutex(t_simulation *sim)
{
	sim->is_running = 1;
	sim->log_mutex_init = 0;
	sim->lock_init = 0;
	sim->dongle_lock_init = 0;
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	sim->log_mutex_init = 1;
	if (pthread_mutex_init(&sim->lock, NULL) != 0)
		return (1);
	sim->lock_init = 1;
	if (pthread_mutex_init(&sim->dongle_lock, NULL) != 0)
		return (1);
	sim->dongle_lock_init = 1;
	return (0);
}

int	init_memory(t_simulation *sim)
{
	int	n;

	n = sim->args.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	if (!sim->coders)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->dongles)
	{
		free(sim->coders);
		sim->coders = NULL;
		return (1);
	}
	return (0);
}

int	init_dongles(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->dongles[i].is_taken = 0;
		sim->dongles[i].release_time = 0;
		sim->dongles[i].queue.data = NULL;
		heap_init(&sim->dongles[i].queue, sim->args.number_of_coders);
		i++;
	}
	return (0);
}

int	init_coders(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_time = sim->start_time;
		sim->coders[i].wait_value = -1;
		// sim->coders[i].holding_dongle = NULL;
		sim->coders[i].time_mutex_init = 0;
		sim->coders[i].compile_mutex_init = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].time_mutex, NULL) != 0)
			return (1);
		sim->coders[i].time_mutex_init = 1;
		if (pthread_mutex_init(&sim->coders[i].compile_mutex, NULL) != 0)
			return (1);
		sim->coders[i].compile_mutex_init = 1;
		i++;
	}
	return (0);
}
