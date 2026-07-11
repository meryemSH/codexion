/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:45:48 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/11 15:46:24 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	release_both_dongles(t_simulation *sim, t_coder *coder)
{
	release_dongle(sim, coder->left_dongle);
	if (coder->left_dongle != coder->right_dongle)
		release_dongle(sim, coder->right_dongle);
}

void	*routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (get_isrunning(sim))
	{
		if (coder_finished(coder))
			return (NULL);
		coder->wait_value = get_waiter_value(coder);
		register_waiter(coder);
		while (get_isrunning(sim) && !check_take_dongles(coder))
			;
		if (!get_isrunning(sim))
		{
			release_both_dongles(sim, coder);
			return (NULL);
		}
		if (!do_compile_cycle(coder, sim))
			return (NULL);
	}
	return (NULL);
}

static int	start_monitor(t_simulation *sim, int n)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		pthread_mutex_lock(&sim->lock);
		sim->is_running = 0;
		pthread_mutex_unlock(&sim->lock);
		i = 0;
		while (i < n)
		{
			pthread_join(sim->coders[i].thread, NULL);
			i++;
		}
		return (1);
	}
	return (0);
}

int	start_threads(t_simulation *sim)
{
	int	i;
	int	n;

	i = 0;
	n = sim->args.number_of_coders;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, &routine,
				&sim->coders[i]) != 0)
		{
			pthread_mutex_lock(&sim->lock);
			sim->is_running = 0;
			pthread_mutex_unlock(&sim->lock);
			while (--i >= 0)
				pthread_join(sim->coders[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (start_monitor(sim, n));
}
