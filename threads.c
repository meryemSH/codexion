#include "codexion.h"

void	*routine(void *arg)
{
	t_coder		*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	log_action(sim, coder->id, "started");
	while (sim->is_running)
		usleep(1000);
	return (NULL);
}

int	start_threads(t_simulation *sim)
{
	int			i;
	int			n;
	pthread_t	monitor_thread;

	i = 0;
	n = sim->args.number_of_coders;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, routine,
				&sim->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, sim) != 0)
		return (1);
	pthread_join(monitor_thread, NULL);
	return (0);
}

void	join_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}
