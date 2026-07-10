/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:52:54 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/10 16:23:39 by mseghrou         ###   ########.fr       */
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

static int	waiter_is_unblocked(t_waiter *w)
{
	if (!dongle_ready(w->first))
		return (0);
	if (w->second != w->first && !dongle_ready(w->second))
		return (0);
	return (1);
}

static int	queue_blocks(t_dongle *d, t_coder *c, t_waiter *self)
{
	int			i;
	t_waiter	*w;

	i = 0;
	while (i < d->queue.size)
	{
		w = &d->queue.data[i];
		if (w->coder_id != c->id && waiter_less(w, self)
			&& waiter_is_unblocked(w))
			return (1);
		i++;
	}
	return (0);
}

static int	can_take_both(t_coder *c, t_dongle *first, t_dongle *second,
		t_waiter *self)
{
	if (!dongle_ready(first))
		return (0);
	if (second != first && !dongle_ready(second))
		return (0);
	if (queue_blocks(first, c, self))
		return (0);
	if (second != first && queue_blocks(second, c, self))
		return (0);
	return (1);
}

int	try_take_both(t_coder *c)
{
	t_dongle		*first;
	t_dongle		*second;
	t_waiter		self;
	t_simulation	*sim;

	sim = c->sim;
	first = first_dongle(c);
	second = second_dongle(c);
	self.coder_id = c->id;
	self.value = c->wait_value;
	self.tie_id = c->id;
	pthread_mutex_lock(&sim->dongle_lock);
	if (!can_take_both(c, first, second, &self))
	{
		pthread_mutex_unlock(&sim->dongle_lock);
		return (0);
	}
	first->is_taken = 1;
	if (second != first)
		second->is_taken = 1;
	remove_waiter(first, c->id);
	if (second != first)
		remove_waiter(second, c->id);
	pthread_mutex_unlock(&sim->dongle_lock);
	return (1);
}

void	release_dongle(t_simulation *sim, t_dongle *d)
{
	pthread_mutex_lock(&sim->dongle_lock);
	d->is_taken = 0;
	d->release_time = get_time() + sim->args.dongle_cooldown;
	pthread_mutex_unlock(&sim->dongle_lock);
}
