#include "codexion.h"
#include <unistd.h>

static int	simulation_running(t_simulation *sim)
{
	int	running;

	pthread_mutex_lock(&sim->log_mutex);
	running = sim->is_running;
	pthread_mutex_unlock(&sim->log_mutex);
	return (running);
}

static int	check_burnout(t_simulation *sim, int i)
{
	long	elapsed;

	pthread_mutex_lock(&sim->log_mutex);
	elapsed = get_time() - sim->coders[i].last_compile_time;
	pthread_mutex_unlock(&sim->log_mutex);
	if (elapsed >= sim->args.time_to_burnout)
	{
		log_action(sim, sim->coders[i].id, "burned out");
		pthread_mutex_lock(&sim->log_mutex);
		sim->is_running = 0;
		pthread_mutex_unlock(&sim->log_mutex);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	while (simulation_running(sim))
	{
		i = 0;
		while (i < sim->args.number_of_coders)
		{
			if (check_burnout(sim, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}