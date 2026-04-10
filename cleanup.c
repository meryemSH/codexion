#include "codexion.h"

void	cleanup(t_simulation *sim)
{
	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
		free(sim->dongles);
}