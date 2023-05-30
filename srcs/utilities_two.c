/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 09:31:22 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/30 13:43:03 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return(current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

suseconds_t	get_current_time_micro(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return(current_time.tv_sec * 1000000 + current_time.tv_usec);
}

int	please_wait(int milli_to_wait, t_phil *phil)
{
	suseconds_t	end;
	// long long end;

	end = get_current_time_micro() + (milli_to_wait * 1000);
	// end = get_current_time() + (milli_to_wait);
	while (get_current_time_micro() < end)
	{
		usleep(500);
		// if (phil && check_vitals(phil) == DEAD)
		// {
		// 	pthread_mutex_unlock(&(phil->left_fork));
		// 	if (phil->right_fork)
		// 		pthread_mutex_unlock(phil->right_fork);
		// 	return (DEAD);
		// }
		
	}
	// printf("FROM WAIT %i\n", end - get_current_time_micro());
	return (check_vitals(phil));
	// return (ALIVE);
}

void	protected_print(t_phil *phil, char *msg, int lock)
{
	if (lock == UNLOCK)
	{
		pthread_mutex_lock(&(phil->shared->print_mutex));
		if (check_vitals(phil) == DEAD)
		{
			pthread_mutex_unlock(&(phil->shared->print_mutex));
			return ;
		}
		printf("%lli %d %s\n", ((get_current_time() - phil->shared->start)) , phil->id, msg);
		pthread_mutex_unlock(&(phil->shared->print_mutex));
		return ;
	}
	else if (lock == LOCK)
	{
		pthread_mutex_lock(&(phil->shared->print_mutex));
		printf("%lli %d %s\n", ((get_current_time() - phil->shared->start)) , phil->id, msg);
		return ;
	}
}
