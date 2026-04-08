#include "codexion.h"

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;

	printf("Coder %d started\n", coder->id);
	return (NULL);
}

int	start_threads(t_simulation *sim)
{
	int	i;
	int	n;

	i = 0;
	n = sim->args.number_of_coders;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				routine, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
void join_threads(t_simulation *sim)
{
    int i;

    i = 0;
    while (i < sim->args.number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
}