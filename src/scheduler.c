/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:46:18 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/11 15:46:20 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_waiter_value(t_coder *c)
{
	t_simulation	*sim;
	long			last;

	sim = c->sim;
	if (strcmp(sim->args.scheduler, "fifo") == 0)
		return (get_time());
	if (strcmp(sim->args.scheduler, "edf") == 0)
	{
		pthread_mutex_lock(&c->time_mutex);
		last = c->last_compile_time;
		pthread_mutex_unlock(&c->time_mutex);
		return (last + sim->args.time_to_burnout);
	}
	return (0);
}
