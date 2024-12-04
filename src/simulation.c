/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:27:51 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:36:32 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*monitor(void *arg)
{
	int		i;
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (check_stop(table))
			break ;
		i = 0;
		while (i < table->num_philosophers)
		{
			if (check_philosopher_death(table, i))
				return (NULL);
			i++;
		}
		usleep(50);
	}
	return (NULL);
}

static int	create_philosopher_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		if (pthread_create(&table->philosopher[i].thread, NULL, \
			philosopher_routine, &table->philosopher[i]) != 0)
		{
			printf("Error: Failed create thread for philosopher %d\n", i + 1);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	join_philosopher_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_join(table->philosopher[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_table *table)
{
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor, table) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (0);
	}
	if (!create_philosopher_threads(table))
		return (0);
	pthread_join(monitor_thread, NULL);
	join_philosopher_threads(table);
	return (1);
}

void	cleanup_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_destroy(&table->forks[i].mutex);
		pthread_mutex_destroy(&table->philosopher[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&table->stop_mutex);
	pthread_mutex_destroy(&table->print_lock);
	free(table->forks);
	free(table->philosopher);
}
