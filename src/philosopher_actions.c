/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:55:08 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:15:37 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philosopher_think(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->table->print_lock);
	printf("%ld Philosopher %d is thinking.\n", \
		get_timestamp(), philosopher->id);
	pthread_mutex_unlock(&philosopher->table->print_lock);
}

int	philosopher_eat(t_philosopher *philosopher)
{
	lock_forks(philosopher);
	if (should_stop(philosopher))
	{
		unlock_forks(philosopher);
		return (1);
	}
	pthread_mutex_lock(&philosopher->table->print_lock);
	printf("%ld Philosopher %d is eating.\n", get_timestamp(), philosopher->id);
	pthread_mutex_unlock(&philosopher->table->print_lock);
	pthread_mutex_lock(&philosopher->meal_mutex);
	philosopher->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&philosopher->meal_mutex);
	safe_usleep(philosopher->table->time_to_eat, philosopher);
	unlock_forks(philosopher);
	return (should_stop(philosopher));
}

void	philosopher_sleep(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->table->print_lock);
	printf("%ld Philosopher %d is sleeping.\n", \
		get_timestamp(), philosopher->id);
	pthread_mutex_unlock(&philosopher->table->print_lock);
	safe_usleep(philosopher->table->time_to_sleep, philosopher);
}

void	safe_usleep(int duration_ms, t_philosopher *philosopher)
{
	int	elapsed;

	elapsed = 0;
	while (elapsed < duration_ms)
	{
		if (should_stop(philosopher))
			break ;
		usleep(1000);
		elapsed += 1;
	}
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	printf("Philosopher %d thread started.\n", philosopher->id);
	while (!should_stop(philosopher))
	{
		philosopher_think(philosopher);
		if (should_stop(philosopher))
			break ;
		if (philosopher_eat(philosopher))
			break ;
		philosopher_sleep(philosopher);
	}
	printf("Philosopher %d thread exiting.\n", philosopher->id);
	return (NULL);
}
