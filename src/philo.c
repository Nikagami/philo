/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aafounas <aafounas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 17:17:15 by aafounas          #+#    #+#             */
/*   Updated: 2024/10/21 13:36:43 by aafounas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int initialize_philosophers(t_table *table)
{
    int i;

    table->forks = malloc(sizeof(t_fork) * table->num_philosophers);
    table->philosopher = malloc(sizeof(t_philosopher) * table->num_philosophers);
    if (!table->forks || !table->philosopher)
    {
        printf("Error: Memory allocation failed.\n");
        if (table->forks)
            free(table->forks);
        if (table->philosopher)
            free(table->philosopher);
        return (0);
    }
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
    i = 0;
    while (i < table->num_philosophers)
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
        table->philosopher[i].r_fork = &table->forks[(i + 1) % table->num_philosophers];
        table->philosopher[i].table = table;
        table->philosopher[i].last_meal_time = get_timestamp();
        i++;
    }
    return (1);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher;

    philosopher = (t_philosopher *)arg;
    printf("Philosopher %d thread started.\n", philosopher->id);
    while (1)
    {
        pthread_mutex_lock(&philosopher->table->stop_mutex);
        if (philosopher->table->stop)
        {
            pthread_mutex_unlock(&philosopher->table->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->table->stop_mutex);
        pthread_mutex_lock(&philosopher->table->print_lock);
        printf("%ld Philosopher %d is thinking.\n", get_timestamp(), philosopher->id);
        pthread_mutex_unlock(&philosopher->table->print_lock);
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
        pthread_mutex_lock(&philosopher->table->print_lock);
        printf("%ld Philosopher %d is eating.\n", get_timestamp(), philosopher->id);
        pthread_mutex_unlock(&philosopher->table->print_lock);
        pthread_mutex_lock(&philosopher->meal_mutex);
        philosopher->last_meal_time = get_timestamp();
        pthread_mutex_unlock(&philosopher->meal_mutex);
        usleep(philosopher->table->time_to_eat * 1000);
        pthread_mutex_unlock(&philosopher->r_fork->mutex);
        pthread_mutex_unlock(&philosopher->l_fork->mutex);
        pthread_mutex_lock(&philosopher->table->print_lock);
        printf("%ld Philosopher %d is sleeping.\n", get_timestamp(), philosopher->id);
        pthread_mutex_unlock(&philosopher->table->print_lock);
        usleep(philosopher->table->time_to_sleep * 1000);
    }
    printf("Philosopher %d thread exiting.\n", philosopher->id);
    return NULL;
}

void *monitor(void *arg) 
{
    t_table *table;
    long last_meal_time;
    long current_time;
    int i;
    
    table = (t_table *)arg;
    while (1) 
    {
        i = 0;
        current_time = get_timestamp();
        pthread_mutex_lock(&table->stop_mutex);
        if (table->stop) 
        {
            pthread_mutex_unlock(&table->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&table->stop_mutex);
        while (i < table->num_philosophers) 
        {
            pthread_mutex_lock(&table->philosopher[i].meal_mutex);
            last_meal_time = table->philosopher[i].last_meal_time;
            pthread_mutex_unlock(&table->philosopher[i].meal_mutex);
            if (current_time - last_meal_time > table->time_to_die) 
            {
                pthread_mutex_lock(&table->print_lock);
                printf("%ld Philosopher %d died\n", current_time, table->philosopher[i].id);
                pthread_mutex_unlock(&table->print_lock);
                pthread_mutex_lock(&table->stop_mutex);
                table->stop = 1;
                pthread_mutex_unlock(&table->stop_mutex);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
}

int start_simulation(t_table *table)
{
    pthread_t monitor_thread;
    int i;

    printf("Starting somulation with %d philosophers.\n", table->num_philosophers);
    if (pthread_create(&monitor_thread, NULL, monitor, table) != 0)
    {
        printf("Error: Failed to create monitor thread\n");
        return (0);
    }
    i = 0;
    while (i < table->num_philosophers)
    {
        if (pthread_create(&table->philosopher[i].thread,\
        NULL, philosopher_routine, &table->philosopher[i]) != 0)
        {
            printf("Error: Failed to create thread for philosopher %d\n", i + 1);
            return (0);
        }
        printf("Thread for philosopher %d created successfully.\n", i + 1);
        i++;
    }
    i = 0;
    while (i < table->num_philosophers)
    {
        pthread_join(table->philosopher[i].thread, NULL);
        i++;
    }
    pthread_join(monitor_thread, NULL);
    return (1);
}

void cleanup_simulation(t_table *table)
{
    int i;

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

