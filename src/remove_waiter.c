/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_waiter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:46:07 by mseghrou          #+#    #+#             */
/*   Updated: 2026/07/11 15:46:09 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	remove_waiter(t_dongle *d, int id)
{
	int	i;

	i = 0;
	while (i < d->queue.size)
	{
		if (d->queue.data[i].coder_id == id)
		{
			d->queue.data[i] = d->queue.data[d->queue.size - 1];
			d->queue.size--;
			if (i < d->queue.size)
			{
				bubble_up(&d->queue, i);
				bubble_down(&d->queue, i);
			}
			return ;
		}
		i++;
	}
}
