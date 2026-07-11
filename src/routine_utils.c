/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:46:13 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/11 15:46:15 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *c)
{
	if (c->left_dongle == c->right_dongle)
	{
		pthread_mutex_lock(&c->sim->dongle_lock);
		if (dongle_ready(c->left_dongle))
		{
			c->left_dongle->is_taken = 1;
			log_action(c->sim, c->id, "has taken a dongle");
		}
		pthread_mutex_unlock(&c->sim->dongle_lock);
		return (0);
	}
	if (!try_take_both(c))
		return (0);
	log_action(c->sim, c->id, "has taken a dongle");
	log_action(c->sim, c->id, "has taken a dongle");
	pthread_mutex_lock(&c->time_mutex);
	c->last_compile_time = get_time();
	pthread_mutex_unlock(&c->time_mutex);
	return (1);
}

int	coder_finished(t_coder *c)
{
	t_simulation	*sim;
	int				count;

	sim = c->sim;
	if (sim->args.number_of_compiles_required <= 0)
		return (0);
	pthread_mutex_lock(&c->compile_mutex);
	count = c->compile_count;
	pthread_mutex_unlock(&c->compile_mutex);
	if (count >= sim->args.number_of_compiles_required)
		return (1);
	return (0);
}

void	smart_sleep(long time, t_simulation *sim)
{
	long	start;

	start = get_time();
	while (get_time() - start < time && get_isrunning(sim) == true)
		usleep(500);
}
