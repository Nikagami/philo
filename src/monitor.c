/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:34:56 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:06:42 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_stop(t_table *table)
{
	int	stop;

	pthread_mutex_lock(&table->stop_mutex);
	stop = table->stop;
	pthread_mutex_unlock(&table->stop_mutex);
	return (stop);
}

int	check_philosopher_death(t_table *table, int index)
{
	long	last_meal_time;
	long	current_time;

	pthread_mutex_lock(&table->philosopher[index].meal_mutex);
	last_meal_time = table->philosopher[index].last_meal_time;
	pthread_mutex_unlock(&table->philosopher[index].meal_mutex);
	current_time = get_timestamp();
	if (current_time - last_meal_time > table->time_to_die)
	{
		pthread_mutex_lock(&table->print_lock);
		printf("\033[91m%ld Philosopher %d died\033[0m\n", \
			current_time, table->philosopher[index].id);
		pthread_mutex_unlock(&table->print_lock);
		pthread_mutex_lock(&table->stop_mutex);
		table->stop = 1;
		pthread_mutex_unlock(&table->stop_mutex);
		return (1);
	}
	return (0);
}
