/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseghrou <mseghrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 11:53:23 by mseghrou          #+#    #+#             */
/*   Updated: 2026/06/11 00:01:16 by mseghrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (i > 10)
		return (0);
	if (i == 10 && strcmp(str, "2147483647") > 0)
		return (0);
	return (1);
}

static void	to_lowercase(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

static int	check_numeric_args(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_positive(argv[i]))
		{
			printf("Error: argument %d must be a positive integer\n", i);
			return (0);
		}
		i++;
	}
	if (atoi(argv[1]) <= 0)
	{
		printf("Error: number_of_coders must be at least 1\n");
		return (0);
	}
	if (atoi(argv[2]) <= 0)
	{
		printf("Error: time_to_burnout must be at least 1\n");
		return (0);
	}
	if (atoi(argv[3]) <= 0)
	{
		printf("Error: time_to_compile must be at least 1\n");
		return (0);
	}
	if (atoi(argv[4]) <= 0)
	{
		printf("Error: time_to_debug must be at least 1\n");
		return (0);
	}
	if (atoi(argv[5]) <= 0)
	{
		printf("Error: time_to_refactor must be at least 1\n");
		return (0);
	}
	if (atoi(argv[6]) <= 0)
	{
		printf("Error: number_of_compiles_required must be at least 1\n");
		return (0);
	}
	return (1);
}

int	check_args(int argc, char **argv)
{
	if (argc != 9)
	{
		printf("Error: expected 8 arguments\n");
		return (0);
	}
	if (!check_numeric_args(argv))
		return (0);
	to_lowercase(argv[8]);
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
