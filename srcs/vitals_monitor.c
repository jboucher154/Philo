/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vitals_monitor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:27:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/06/05 13:49:53 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_diner(t_diner *diner)
{
	int	i;

	i = 0;
	while (i < diner->shared->nb_philo)
	{
		pthread_mutex_lock(&(diner->all_the_phils[i]->vitals_mutex));
		diner->all_the_phils[i]->vital_sign = DEAD;
		pthread_mutex_unlock(&(diner->all_the_phils[i]->vitals_mutex));
		i++;
	}
}

int	all_full(t_diner *diner)
{
	pthread_mutex_lock(&(diner->shared->full_mutex));
	if (diner->shared->full_phils == diner->shared->nb_philo)
	{
		pthread_mutex_unlock(&(diner->shared->full_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(diner->shared->full_mutex));
	return (0);
}

void	kill_phil(t_diner *diner, int index)
{
	pthread_mutex_lock(&(diner->all_the_phils[index]->vitals_mutex));
	diner->all_the_phils[index]->vital_sign = DEAD;
	pthread_mutex_unlock(&(diner->all_the_phils[index]->vitals_mutex));
	protected_print(diner->all_the_phils[index], "died", LOCK);
}

void	vitals_monitor(t_diner *diner)
{
	int			i;
	long long	delta;

	i = 0;
	while (i < diner->shared->nb_philo)
	{
		pthread_mutex_lock(&(diner->all_the_phils[i]->meal_mutex));
		delta = get_current_time() - diner->all_the_phils[i]->last_meal;
		if (delta >= 0)
		{
			pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
			kill_phil(diner, i);
			break ;
		}
		pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
		if (diner->shared->nb_eat != UNSET && all_full(diner))
			break ;
		i++;
		if (i == diner->shared->nb_philo)
			i = 0;
	}
	close_diner(diner);
	pthread_mutex_unlock(&(diner->shared->print_mutex));
}
