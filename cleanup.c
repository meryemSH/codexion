#include "codexion.h"

void cleanup(t_simulation *sim)
{
    int i;

    pthread_mutex_destroy(&sim->log_mutex);
    pthread_mutex_destroy(&sim->lock);
    if (sim->dongles)
    {
        i = 0;
        while (i < sim->args.number_of_coders)
            pthread_mutex_destroy(&sim->dongles[i++].mutex);
        free(sim->dongles);
    }
    if (sim->coders)
        free(sim->coders);
}
