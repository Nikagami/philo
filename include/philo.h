/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:49:07 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:31:43 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philosopher
{
	int				id;
	t_fork			*l_fork;
	t_fork			*r_fork;
	pthread_t		thread;
	int				meal_eaten;
	struct s_table	*table;
	long			last_meal_time;
	pthread_mutex_t	meal_mutex;
}	t_philosopher;

typedef struct s_table
{
	int				num_philosophers;
	t_philosopher	*philosopher;
	t_fork			*forks;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	t_fork			*fork;
	long			start_time;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_mutex;
	int				stop;
}	t_table;

int		check_args(int ac, char **av);
int		parse_args(t_table *table, int ac, char **av);
long	get_timestamp(void);
void	*philosopher_routine(void *arg);
int		start_simulation(t_table *table);
void	cleanup_simulation(t_table *table);
int		initialize_philosophers(t_table *table);
int		init_mutexes(t_table *table);
int		init_philosopher_data(t_table *table, int i);
void	philosopher_think(t_philosopher *philosopher);
int		philosopher_eat(t_philosopher *philosopher);
void	philosopher_sleep(t_philosopher *philosopher);
int		should_stop(t_philosopher *philosopher);
void	lock_forks(t_philosopher *philosopher);
void	unlock_forks(t_philosopher *philosopher);
void	safe_usleep(int duration_ms, t_philosopher *philosopher);
int		check_stop(t_table *table);
int		check_philosopher_death(t_table *table, int index);

#endif