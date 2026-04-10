// #include "../codexion.h"

// int	main(int argc, char **argv)
// {
// 	t_simulation	sim;

// 	if (!check_args(argc, argv))
// 		return (1);
// 	fill_args(argv, &sim.args);
// 	if (init_simulation(&sim))
// 	{
// 		fprintf(stderr, "Error: init failed\n");
// 		return (1);
// 	}
// 	if (start_threads(&sim))
// 	{
// 		fprintf(stderr, "Error: threads failed\n");
// 		cleanup(&sim);
// 		return (1);
// 	}
// 	join_threads(&sim);
// 	cleanup(&sim);
// 	return (0);
// }


#include <stdio.h>
#include <pthread.h>

int counter = 0;

void *worker(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        counter++;
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);

    printf("Final counter = %d\n", counter);
    return 0;
}