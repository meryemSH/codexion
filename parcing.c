#include "codexion.h"

int	is_positive(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("Error: expected 8 arguments\n");
		return (0);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_positive(argv[i]))
		{
			printf("Error: argument %d must be a positive integer\n",
				i);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("Error: scheduler must be 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

void	fill_args(char **argv, t_args *args)
{
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]);
	args->time_to_debug = atoi(argv[4]);
	args->time_to_refactor = atoi(argv[5]);
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	strncpy(args->scheduler, argv[8], 4);
	args->scheduler[4] = '\0';
}

// int	main(int argc, char **argv)
// {
// 	t_args	args;

// 	if (!check_args(argc, argv))
// 		return (1);
// 	fill_args(argv, &args);
// 	printf("Coders: %d | Scheduler: %s\n", args.number_of_coders,
// 		args.scheduler);
// 	return (0);
// }
