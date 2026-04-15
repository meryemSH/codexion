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
	sim->start_time = get_time();
	return (0);
}
int	init_mutex(t_simulation *sim)
{
	sim->is_running = 1;
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->lock, NULL) != 0)
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
		sim->coders[i].last_compile_time = sim->start_time;
		sim->coders[i].is_alive = 1;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		sim->coders[i].sim = sim; // exp in bas 
		i++;
	}
	return (0);
}
// sim->coders[i].sim = sim; (chouf struct dyal coder w sim)

// `pthread_create` kayt3tik ghi **wahed argument** — `void *arg`.

// F routine, had l arg howa `t_coder *` — donc:
// ```c
// coder = (t_coder *)arg;
// ```

// Daba ila routine khassها twsl l simulation — mn fin?

// **Mn coder!** Donc zidna `sim` f struct dyal coder:
// ```c
// coder->sim  →  twsl l simulation
// ```

// Bla hadi, routine ma3ndahash access l simulation.

// Fhmti?