/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:11:10 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/11 15:11:11 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*first_dongle(t_coder *c)
{
	if (c->left_dongle < c->right_dongle)
		return (c->left_dongle);
	return (c->right_dongle);
}

t_dongle	*second_dongle(t_coder *c)
{
	if (c->left_dongle < c->right_dongle)
		return (c->right_dongle);
	return (c->left_dongle);
}

int	dongle_ready(t_dongle *d)
{
	return (!d->is_taken && get_time() >= d->release_time);
}

static int	is_already_waiting(t_dongle *d, int id)
{
	int	i;

	i = 0;
	while (i < d->queue.size)
	{
		if (d->queue.data[i].coder_id == id)
			return (1);
		i++;
	}
	return (0);
}

void	register_waiter(t_coder *c)
{
	t_waiter		w;
	t_simulation	*sim;

	sim = c->sim;
	w.coder_id = c->id;
	w.value = c->wait_value;
	w.tie_id = c->id;
	w.first = first_dongle(c);
	w.second = second_dongle(c);
	pthread_mutex_lock(&sim->dongle_lock);
	if (!is_already_waiting(w.first, c->id))
		heap_push(&w.first->queue, w);
	if (w.second != w.first && !is_already_waiting(w.second, c->id))
		heap_push(&w.second->queue, w);
	pthread_mutex_unlock(&sim->dongle_lock);
}
