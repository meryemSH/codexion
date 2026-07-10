/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:53:20 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/10 20:19:25 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static int	check_burnout(t_simulation *sim)
{
	int		i;
	long	over;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (coder_finished(&sim->coders[i]))
		{
			i++;
			continue ;
		}
		pthread_mutex_lock(&sim->coders[i].time_mutex);
		over = get_time() - sim->coders[i].last_compile_time;
		pthread_mutex_unlock(&sim->coders[i].time_mutex);
		if (over >= sim->args.time_to_burnout)
		{
			pthread_mutex_lock(&sim->lock);
			sim->is_running = 0;
			pthread_mutex_unlock(&sim->lock);
			log_action(sim, sim->coders[i].id, "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	all_coders_finished(t_simulation *sim)
{
	int	i;
	int	count;

	i = 0;
	if (sim->args.number_of_compiles_required <= 0)
		return (0);
	while (i < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].compile_mutex);
		count = sim->coders[i].compile_count;
		pthread_mutex_unlock(&sim->coders[i].compile_mutex);
		if (count < sim->args.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (1)
	{
		if (!get_isrunning(sim))
			return (NULL);
		if (check_burnout(sim))
			return (NULL);
		if (all_coders_finished(sim))
		{
			pthread_mutex_lock(&sim->lock);
			sim->is_running = 0;
			pthread_mutex_unlock(&sim->lock);
			return (NULL);
		}
	}
}
