/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vitals_monitor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:27:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/26 11:50:02 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_diner(t_diner *diner)
{
	int i;

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
	int 	i;
	long long	delta;

	i = 0;
	// printf("\nMonitoring vitals......\n\n");//
	while (i < diner->shared->nb_philo)
	{
		// get_current_time();
		// printf("%lli\n", get_current_time());
		// printf("asdf %lli\n", get_current_time());
		// usleep(300);
		// printf("CHECK 1phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
		// printf("CHECK 1phil: %i, time: %lli, \n", i + 1, get_current_time());
		pthread_mutex_lock(&(diner->all_the_phils[i]->meal_mutex));
		// printf("CHECK 2phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
		// printf("MEAL CHECK %i, time of meal %llu, death timer%i\n", get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die, get_current_time() - diner->all_the_phils[i]->last_meal, diner->shared->time_to_die);
		// if (get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die)
		// suseconds_t time_to_die = diner->all_the_phils[i]->last_meal;
		// suseconds_t current_time = get_current_time_micro();
		// suseconds_t delta_time = current_time - time_to_die;
		// printf("CHECK 2phil: %i, state: %i, time to die: %i time: %i, delta: %i \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
		// if (diner->all_the_phils[i]->last_meal <= get_current_time_micro())
		delta = get_current_time() - diner->all_the_phils[i]->last_meal;
		if (delta >= 0)
		{
			pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
			// kill_phil(diner, i);
			// long long int time_to_die = diner->all_the_phils[i]->last_meal;
			// long long int current_time = get_current_time();
			// long long int delta_time = current_time - time_to_die;
			// printf("CHECK 3phil: %i, state: %i, time to die: %lli time: %lli, delta: %lli \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
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
	// printf("\nClosing diner......\n\n");//
	close_diner(diner);
	pthread_mutex_unlock(&(diner->shared->print_mutex));
}
///////////////////
// void	*vitals_monitor_one(void *diner_to_cast)
// {
// 	int 	i;
// 	t_diner	*diner;

// 	i = 0;
// 	diner = (t_diner *)diner_to_cast;
// 	// printf("\nMonitoring vitals......\n\n");//
// 	while (i < diner->shared->nb_philo / 2)
// 	{
// 		// get_current_time();
// 		// printf("%lli\n", get_current_time());
// 		// printf("asdf %lli\n", get_current_time());
// 		usleep(200);
// 		if (check_vitals(diner->all_the_phils[i]) == DEAD)
// 			break ;
// 		// printf("CHECK 1phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
// 		// printf("CHECK 1phil: %i, time: %lli, \n", i + 1, get_current_time());
// 		pthread_mutex_lock(&(diner->all_the_phils[i]->meal_mutex));
// 		// printf("CHECK 2phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
// 		// printf("MEAL CHECK %i, time of meal %llu, death timer%i\n", get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die, get_current_time() - diner->all_the_phils[i]->last_meal, diner->shared->time_to_die);
// 		// if (get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die)
// 		long long int time_to_die = diner->all_the_phils[i]->last_meal;
// 		long long int current_time = get_current_time();
// 		long long int delta_time = current_time - time_to_die;
// 		printf("CHECK 2phil: %i, state: %i, time to die: %lli time: %lli, delta: %lli \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
// 		if (diner->all_the_phils[i]->last_meal <= get_current_time())
// 		{
// 			pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
// 			// kill_phil(diner, i);
// 			long long int time_to_die = diner->all_the_phils[i]->last_meal;
// 			long long int current_time = get_current_time();
// 			long long int delta_time = current_time - time_to_die;
// 			printf("CHECK 3phil: %i, state: %i, time to die: %lli time: %lli, delta: %lli \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
// 			kill_phil(diner, i);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
// 		if (diner->shared->nb_eat != UNSET && all_full(diner))
// 			break ;
// 		i++;
// 		if (i == diner->shared->nb_philo / 2)
// 			i = 0;
// 	}
// 	// printf("\nClosing diner......\n\n");//
// 	close_diner(diner);
// 	pthread_mutex_unlock(&(diner->shared->print_mutex));
// 	return (diner_to_cast);
// }

// void	*vitals_monitor_two(void *diner_to_cast)
// {
// 	int 	i;
// 	t_diner	*diner;

// 	diner = (t_diner *)diner_to_cast;
// 	i = diner->shared->nb_philo / 2;
// 	// printf("\nMonitoring vitals......\n\n");//
// 	while (i < diner->shared->nb_philo)
// 	{
// 		usleep(200);
// 		if (check_vitals(diner->all_the_phils[i]) == DEAD)
// 			break ;
// 		// printf("CHECK 1phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
// 		pthread_mutex_lock(&(diner->all_the_phils[i]->meal_mutex));
// 		// printf("CHECK 2phil: %i, time to die: %lli time: %lli, \n", i + 1, diner->all_the_phils[i]->last_meal, get_current_time());
// 		// printf("MEAL CHECK %i, time of meal %llu, death timer%i\n", get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die, get_current_time() - diner->all_the_phils[i]->last_meal, diner->shared->time_to_die);
// 		// get_current_time() - diner->all_the_phils[i]->last_meal >=  diner->shared->time_to_die
// 		long long int time_to_die = diner->all_the_phils[i]->last_meal;
// 		long long int current_time = get_current_time();
// 		long long int delta_time = current_time - time_to_die;
// 		printf("CHECK 2phil: %i, state: %i, time to die: %lli time: %lli, delta: %lli \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
			
// 		if (diner->all_the_phils[i]->last_meal <= get_current_time())
// 		{
// 			pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
// 			long long int time_to_die = diner->all_the_phils[i]->last_meal;
// 			long long int current_time = get_current_time();
// 			long long int delta_time = current_time - time_to_die;
// 			printf("CHECK 3phil: %i, state: %i, time to die: %lli time: %lli, delta: %lli \n", i + 1, diner->all_the_phils[i]->state, time_to_die, current_time, delta_time);
// 			kill_phil(diner, i);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&(diner->all_the_phils[i]->meal_mutex));
// 		if (diner->shared->nb_eat != UNSET && all_full(diner))
// 			break ;
// 		i++;
// 		if (i == diner->shared->nb_philo)
// 			i = diner->shared->nb_philo / 2;
// 	}
// 	// printf("\nClosing diner......\n\n");//
// 	close_diner(diner);
// 	pthread_mutex_unlock(&(diner->shared->print_mutex));
// 	return (diner_to_cast);
// }
