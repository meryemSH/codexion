#include "codexion.h"
#include <unistd.h>

static int	simulation_running(t_simulation *sim)
{
	int	running;

	pthread_mutex_lock(&sim->lock);
	running = sim->is_running;
	pthread_mutex_unlock(&sim->lock);
	return (running);
}

static int	check_burnout(t_simulation *sim)
{
	int		i;
	long	elapsed;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		elapsed = get_time() - sim->coders[i].last_compile_time;
		if (elapsed >= sim->args.time_to_burnout)
		{
			sim->is_running = 0;
			log_action(sim, sim->coders[i].id, "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (1)
	{
		if (!simulation_running(sim))
			return (NULL);
		pthread_mutex_lock(&sim->lock);
		if (check_burnout(sim))
		{
			pthread_mutex_unlock(&sim->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->lock);
		usleep(1000);
	}
}