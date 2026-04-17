#include "codexion.h"

int	get_isrunning(t_simulation *sim)
{
	int	value;

	pthread_mutex_lock(&sim->lock);
	value = sim->is_running;
	pthread_mutex_unlock(&sim->lock);
	return (value);
}

void	*routine(void *arg)
{
	t_coder		*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	log_action(sim, coder->id, "started");
	while (get_isrunning(sim))
		usleep(1000);
	return (NULL);
}

int	start_threads(t_simulation *sim)
{
	int			i;
	int			n;

	i = 0;
	n = sim->args.number_of_coders;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, routine,
				&sim->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL, monitor, sim) != 0)
		return (1);
	return (0);
}

void	join_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	pthread_join(sim->monitor_thread, NULL);
	while (i < sim->args.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}
