/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 18:41:47 by aafounas          #+#    #+#             */
/*   Updated: 2024/10/03 16:04:05 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int main (int ac, char **av)
{
    t_table table;
    
    if (!check_args(ac, av))
        return (1);
    if (!parse_args(&table, ac, av))
        return (1);
    if (!initialize_philosophers(&table))
    {
        printf(("Error: Failed to initialize phillosophers.\n"));
        return (1);
    }
    printf("Philosophers and forks initialized successfully.\n");
    if (!start_simulation(&table))
    {
        printf("Error: Failed to start the simulation.\n");
        cleanup_simulation(&table);
        return (1);
    }
    cleanup_simulation(&table);
    return (0);
}
