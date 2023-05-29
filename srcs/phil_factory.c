/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_factory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:12:40 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/26 10:44:51 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	get_right_forks(t_diner *diner)
{
	int i;

	i = 0;
	// printf("getting right forks!\n");
	while (diner->shared->nb_philo > 1 && diner->all_the_phils[i])
	{
		if (i == diner->shared->nb_philo - 1)
			diner->all_the_phils[i]->right_fork = &(diner->all_the_phils[0]->left_fork);
		else
			diner->all_the_phils[i]->right_fork = &(diner->all_the_phils[i + 1]->left_fork);
		i++;
	}
}

static int setup_mutexes(t_diner *diner, int i)
{
	if (pthread_mutex_init(&(diner->all_the_phils[i]->meal_mutex), NULL) != SUCCESS)
		return (ERROR);
	if (pthread_mutex_init(&(diner->all_the_phils[i]->vitals_mutex), NULL) != SUCCESS)
	{
		pthread_mutex_destroy(&(diner->all_the_phils[i]->meal_mutex));
		return (ERROR);
	}
	if (pthread_mutex_init(&(diner->all_the_phils[i]->left_fork), NULL) != SUCCESS)
	{
		pthread_mutex_destroy(&(diner->all_the_phils[i]->meal_mutex));
		pthread_mutex_destroy(&(diner->all_the_phils[i]->meal_mutex));
		return (ERROR);
	}
	return (SUCCESS);
}

static int	init_phils(t_diner *diner)
{
	int i;

	i = 0;
	while (diner->all_the_phils[i])
	{
		diner->all_the_phils[i]->shared = diner->shared;
		diner->all_the_phils[i]->id = i + 1;
		diner->all_the_phils[i]->last_meal = 0;
		diner->all_the_phils[i]->vital_sign = ALIVE;//
		diner->all_the_phils[i]->meals_eaten = 0;
		if (setup_mutexes(diner, i) == ERROR)
			print_error("mutex intialization failed");
		diner->all_the_phils[i]->right_fork = NULL;
		i++;
	}
	if (i < diner->shared->nb_philo)
		return (i); // use i to determine how many mutexes to destroy
	return (SUCCESS);
}

static int	phil_malloc(t_diner *diner)
{
	int i;

	i = 0;
	diner->all_the_phils = malloc(sizeof(t_phil *) * (diner->shared->nb_philo + 1));
	if (!diner->all_the_phils)
		return (print_error("malloc failure"));
	while (i < diner->shared->nb_philo)
	{
		diner->all_the_phils[i] = malloc(sizeof(t_phil));
		if (diner->all_the_phils[i] == NULL)
			break;
		i++;
	}
	if (i < diner->shared->nb_philo)
	{
		free_phils(diner);
		return (print_error("malloc failure"));
	}
	else
		diner->all_the_phils[i] = NULL;	
	return (SUCCESS);
}

int	phil_factory(t_diner *diner)
{
	int phils_initialized;
	
	if (phil_malloc(diner) == ERROR)
		return (ERROR);
	phils_initialized = init_phils(diner);
	// printf("PHILS INITIALIZED: %i\n", phils_initialized);
	if (phils_initialized != SUCCESS)
	{
		destroy_phil_mutexes(diner, phils_initialized);
		free_phils(diner);
		return (ERROR);
	}
	get_right_forks(diner);
	return (SUCCESS);
}
