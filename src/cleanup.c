/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:52:45 by mseghrou          #+#    #+#             */
/*   Updated: 2026/06/16 14:38:19 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (sim->coders[i].time_mutex_init)
			pthread_mutex_destroy(&sim->coders[i].time_mutex);
		if (sim->coders[i].compile_mutex_init)
			pthread_mutex_destroy(&sim->coders[i].compile_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

static void	cleanup_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (sim->dongles[i].queue.data)
			free(sim->dongles[i].queue.data);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	cleanup(t_simulation *sim)
{
	if (sim->coders)
		cleanup_coders(sim);
	if (sim->dongles)
		cleanup_dongles(sim);
	if (sim->log_mutex_init)
		pthread_mutex_destroy(&sim->log_mutex);
	if (sim->lock_init)
		pthread_mutex_destroy(&sim->lock);
	if (sim->dongle_lock_init)
		pthread_mutex_destroy(&sim->dongle_lock);
}
