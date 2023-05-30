/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 10:02:04 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/30 11:25:46 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_vitals(t_phil *phil)
{
	pthread_mutex_lock(&(phil->vitals_mutex));
	if (phil->vital_sign == DEAD)
	{
		pthread_mutex_unlock(&(phil->vitals_mutex));
		return (DEAD);
	}
	pthread_mutex_unlock(&(phil->vitals_mutex));
	return (ALIVE);
}

int	eat(t_phil *phil)
{
	int i;

	i = 0;
	while (phil->shared->eating_tasks[i] != NULL)
	{
		if (check_vitals(phil) == DEAD)
			return (DEAD);
		phil->shared->eating_tasks[i](phil);
		if (check_vitals(phil) == DEAD)
			return (DEAD);
	}

	

	return (check_vitals(phil));
}

void	*phil_routine(void *phil_to_cast)
{
	t_phil	*phil;

	phil = phil_to_cast;

	// protected_print(phil, "is thinking", UNLOCK);
	printf("%lli %d %s\n", (get_current_time() - phil->shared->start), phil->id, "is thinking");
	if (phil->id % 2 == 0)
		please_wait(phil->shared->time_to_eat, NULL);
	while (42)
	{
		//eat
		if (eat(phil) == DEAD)
			break ;
		//sleep
		protected_print(phil, "is sleeping", UNLOCK);
		phil->state = 2;//
		if (please_wait(phil->shared->time_to_sleep, phil) == DEAD)
			break ;
		//think
		protected_print(phil, "is thinking", UNLOCK);
		phil->state = 3;//
	}
	// printf("HI from phil %d\n", phil->id);//
	return (phil_to_cast);
}
