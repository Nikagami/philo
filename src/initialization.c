/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:26:25 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:34:56 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	initialize_philosophers(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(t_fork) * table->num_philosophers);
	table->philosopher = malloc(sizeof(t_philosopher) \
		* table->num_philosophers);
	if (!table->forks || !table->philosopher)
	{
		printf("Error: Memory allocation failed.\n");
		free(table->forks);
		free(table->philosopher);
		return (0);
	}
	if (!init_mutexes(table))
		return (0);
	i = 0;
	while (i < table->num_philosophers)
	{
		if (!init_philosopher_data(table, i))
			return (0);
		i++;
	}
	return (1);
}

int	init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->stop_mutex, NULL) != 0)
	{
		printf("Error initializing stop mutex.\n");
		return (0);
	}
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
	{
		printf("Error initializing print mutex.\n");
		pthread_mutex_destroy(&table->stop_mutex);
		return (0);
	}
	return (1);
}

int	init_philosopher_data(t_table *table, int i)
{
	if (pthread_mutex_init(&table->forks[i].mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed for fork %d.\n", i);
		return (0);
	}
	if (pthread_mutex_init(&table->philosopher[i].meal_mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed for philosopher %d.\n", i);
		return (0);
	}
	table->philosopher[i].id = i + 1;
	table->philosopher[i].l_fork = &table->forks[i];
	table->philosopher[i].r_fork = &table->forks[(i + 1) \
		% table->num_philosophers];
	table->philosopher[i].table = table;
	table->philosopher[i].last_meal_time = get_timestamp();
	return (1);
}
