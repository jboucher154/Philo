/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 10:55:53 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/30 11:23:44 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_full(t_phil *phil)
{
	if (phil->shared->nb_eat != UNSET && phil->meals_eaten == phil->shared->nb_eat)
	{
		pthread_mutex_lock(&(phil->shared->full_mutex));
		phil->shared->full_phils += 1;
		pthread_mutex_unlock(&(phil->shared->full_mutex));
	}
	return (check_vitals(phil));
}

int	pick_up_forks(t_phil *phil)
{
	pthread_mutex_lock(&(phil->left_fork));
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	//print fork taken
	protected_print(phil, "has taken a fork", UNLOCK);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	//take right fork
	if (phil->right_fork)
		pthread_mutex_lock(phil->right_fork);
	else
		please_wait(phil->shared->time_to_die, phil);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	protected_print(phil, "has taken a fork", UNLOCK);
	return (check_vitals(phil));
}
int	eat_meal(t_phil *phil)
{
	protected_print(phil, "is eating", UNLOCK);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	pthread_mutex_lock(&(phil->meal_mutex));//update eat time
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	phil->last_meal = get_current_time() + (phil->shared->time_to_die); // could add the time to die to this and just compare it to the current time in the monitor
	pthread_mutex_unlock(&(phil->meal_mutex));
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	please_wait(phil->shared->time_to_eat, phil);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	phil->meals_eaten++;
	return (check_vitals(phil));
}

int	drop_forks(t_phil *phil)
{
	pthread_mutex_unlock(phil->right_fork);
	pthread_mutex_unlock(&(phil->left_fork));
	return (check_vitals(phil));
}
