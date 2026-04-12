#include "codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (!check_args(argc, argv))
		return (1);
	fill_args(argv, &sim.args);
	if (init_simulation(&sim))
	{
		fprintf(stderr, "Error: failed to initialize simulation\n");
		return (1);
	}
	if (start_threads(&sim))
	{
		cleanup(&sim);
		fprintf(stderr, "Error: failed to start threads\n");
		return (1);
	}
	join_threads(&sim);
	cleanup(&sim);
	return (0);
}