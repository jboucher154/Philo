/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:59:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/06/05 13:45:37 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_prog_mutexes(t_diner *diner)
{
	pthread_mutex_destroy(&(diner->shared->full_mutex));
	pthread_mutex_destroy(&(diner->shared->print_mutex));
}

void	destroy_phil_mutexes(t_diner *diner, int to_destroy)
{
	int	i;

	i = 0;
	while (i < to_destroy)
	{
		if (diner->all_the_phils[i])
		{
			pthread_mutex_destroy(&(diner->all_the_phils[i]->meal_mutex));
			pthread_mutex_destroy(&(diner->all_the_phils[i]->vitals_mutex));
			pthread_mutex_destroy(&(diner->all_the_phils[i]->left_fork));
		}
		i++;
	}
}

void	free_phils(t_diner *diner)
{
	int	i;

	i = 0;
	if (!diner->all_the_phils)
		return ;
	while (diner->all_the_phils[i])
	{
		if (diner->all_the_phils[i])
		{
			free(diner->all_the_phils[i]);
			diner->all_the_phils[i] = NULL;
		}
		i++;
	}
	free (diner->all_the_phils);
}

static void	join_threads(t_diner *diner)
{
	int	i;

	i = 0;
	while (i < diner->shared->nb_philo)
	{
		pthread_join(diner->all_the_phils[i]->phil_thread, NULL);
		i++;
	}
}

void	clean_diner(t_diner *diner)
{
	join_threads(diner);
	destroy_phil_mutexes(diner, diner->shared->nb_philo);
	destroy_prog_mutexes(diner);
	free_phils(diner);
}
