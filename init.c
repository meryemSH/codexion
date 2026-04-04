#include "codexion.h"

int	init_simulation(t_simulation *sim)
{
	int i;

	i = 0;
	sim->is_running = 1;
	sim->coders  = malloc(sizeof(t_coder) * sim->args.number_of_coders);
    sim->dongles = malloc(sizeof(t_dongle) * sim->args.number_of_coders);
	if (!sim->coders || !sim->dongles)
        return (1);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	
	return (0);
}