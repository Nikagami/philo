/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:56:47 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:34:44 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	should_stop(t_philosopher *philosopher)
{
	int	stop;

	pthread_mutex_lock(&philosopher->table->stop_mutex);
	stop = philosopher->table->stop;
	pthread_mutex_unlock(&philosopher->table->stop_mutex);
	return (stop);
}

void	lock_forks(t_philosopher *philosopher)
{
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_lock(&philosopher->r_fork->mutex);
		pthread_mutex_lock(&philosopher->l_fork->mutex);
	}
	else
	{
		pthread_mutex_lock(&philosopher->l_fork->mutex);
		pthread_mutex_lock(&philosopher->r_fork->mutex);
	}
}

void	unlock_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(&philosopher->r_fork->mutex);
	pthread_mutex_unlock(&philosopher->l_fork->mutex);
}
