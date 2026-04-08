#include "codexion.h"

int	init_simulation(t_simulation *sim)
{
	if (init_memory(sim))
		return (1);
	if (init_mutex(sim))
	{
		cleanup(sim);
		return (1);
	}
	if (init_dongles(sim))
	{
		cleanup(sim);
		return (1);
	}
	if (init_coders(sim))
	{
		cleanup(sim);
		return (1);
	}
	return (0);
}
int	init_mutex(t_simulation *sim)
{
	sim->is_running = 1;
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	return (0);
}
int	init_memory(t_simulation *sim)
{
	int	n;

	n = sim->args.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->coders || !sim->dongles)
		return (1);
	return (0);
}

int	init_dongles(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->dongles[i].is_taken = 0;
		sim->dongles[i].release_time = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_coders(t_simulation *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_time = 0;
		sim->coders[i].is_alive = 1;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		i++;
	}
	return (0);
}
