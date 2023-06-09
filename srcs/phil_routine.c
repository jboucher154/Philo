/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 10:02:04 by jebouche          #+#    #+#             */
/*   Updated: 2023/06/06 16:40:52 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_vitals(t_phil *phil)
{
	pthread_mutex_lock(&(phil->vitals_mutex));
	if (phil->vital_sign == DEAD)
	{
		pthread_mutex_unlock(&(phil->vitals_mutex));
		pthread_mutex_unlock(&(phil->left_fork));
		if (phil->right_fork)
			pthread_mutex_unlock(phil->right_fork);
		return (DEAD);
	}
	pthread_mutex_unlock(&(phil->vitals_mutex));
	return (ALIVE);
}

static int	set_full(t_phil *phil)
{
	pthread_mutex_lock(&(phil->shared->full_mutex));
	phil->shared->full_phils += 1;
	pthread_mutex_unlock(&(phil->shared->full_mutex));
	return (check_vitals(phil));
}

static int	pick_up_forks(t_phil *phil)
{
	pthread_mutex_lock(&(phil->left_fork));
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	protected_print(phil, "has taken a fork", UNLOCK);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	if (phil->right_fork)
		pthread_mutex_lock(phil->right_fork);
	else
		please_wait(phil->shared->time_to_die, phil);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	protected_print(phil, "has taken a fork", UNLOCK);
	return (check_vitals(phil));
}

static int	eat(t_phil *phil)
{
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	if (pick_up_forks(phil) == DEAD)
		return (DEAD);
	pthread_mutex_lock(&(phil->meal_mutex));
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	phil->last_meal = get_current_time() + (phil->shared->time_to_die);
	protected_print(phil, "is eating", UNLOCK);
	pthread_mutex_unlock(&(phil->meal_mutex));
	please_wait(phil->shared->time_to_eat, phil);
	if (check_vitals(phil) == DEAD)
		return (DEAD);
	pthread_mutex_unlock(phil->right_fork);
	pthread_mutex_unlock(&(phil->left_fork));
	phil->meals_eaten++;
	if (phil->shared->nb_eat != UNSET && phil->meals_eaten == \
	phil->shared->nb_eat)
		set_full(phil);
	return (check_vitals(phil));
}

void	*phil_routine(void *phil_to_cast)
{
	t_phil	*phil;

	phil = phil_to_cast;
	if (phil->id % 2 == 0)
		please_wait(phil->shared->time_to_eat, phil);
	while (42)
	{
		if (eat(phil) == DEAD)
			break ;
		protected_print(phil, "is sleeping", UNLOCK);
		if (please_wait(phil->shared->time_to_sleep, phil) == DEAD)
			break ;
		protected_print(phil, "is thinking", UNLOCK);
	}
	return (phil_to_cast);
}
