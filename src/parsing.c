/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:59:22 by aafounas          #+#    #+#             */
/*   Updated: 2024/12/04 17:34:09 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	valid_number(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

int	check_args(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
	{
		printf("Error : Invalid number of arguments\n");
		return (0);
	}
	while (i < ac)
	{
		if (!valid_number(av[1]))
		{
			printf("Error : Argument %d is not a valid number\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_args(t_table *table, int ac, char **av)
{
	table->stop = 0;
	table->num_philosophers = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]);
	table->time_to_eat = ft_atol(av[3]);
	table->time_to_sleep = ft_atol(av[4]);
	if (table->num_philosophers <= 0 || table->time_to_die <= 0 \
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0)
	{
		printf("Error: Arguments must be positive numbers.\n");
		return (0);
	}
	if (ac == 6)
	{
		table->must_eat_count = ft_atol(av[5]);
		if (table->must_eat_count <= 0)
		{
			printf("Error: The number of meals must be a positive number.\n");
			return (0);
		}
	}
	else
		table->must_eat_count = -1;
	return (1);
}
