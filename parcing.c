#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int	main(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("Error: expected 8 arguments\n");
		return (1);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_positive(argv[i]))
		{
			printf("Error: argument %d must be a positive integer\n", i);
			return (1);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("Error: scheduler must be 'fifo' or 'edf'\n");
		return (1);
	}
	printf("All arguments valid!\n");
	return (0);
}
