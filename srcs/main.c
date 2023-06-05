/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:47:21 by jebouche          #+#    #+#             */
/*   Updated: 2023/06/05 09:26:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	open_diner(t_diner *diner)
{
	int					i;

	i = 0;
	diner->shared->start = get_current_time();
	while (i < diner->shared->nb_philo)
	{
		diner->all_the_phils[i]->last_meal = diner->shared->start + diner->shared->time_to_die;
		if (pthread_create(&(diner->all_the_phils[i]->phil_thread), NULL, phil_routine, diner->all_the_phils[i]))
			return (print_error("Thread creation failed"));
		i++;
	}
	vitals_monitor(diner);
	return (SUCCESS);
}

int	verify_args(t_shared *shared)
{
	if (shared->nb_philo <= 0 || shared->nb_philo > 200)
		return (print_error("number of philosophers must be > 0 and <= 200"));
	if (shared->time_to_die == 0)
		return (print_error("time to die must be an integer > 0"));
	if (shared->time_to_eat == 0)
		return (print_error("time to eat must be an integer > 0"));
	if (shared->time_to_sleep == 0)
		return (print_error("time to sleep must be an integer > 0"));
	if (shared->nb_eat != UNSET && shared->nb_eat == 0)
		return (ERROR);
	else if (shared->nb_eat != UNSET && shared->nb_eat < 0)
		return (print_error("meals to eat must be an integer > 0"));
	return (SUCCESS);
}

int	process_args(int argc, char **argv, t_shared *shared)
{
	shared->nb_philo = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		shared->nb_eat = ft_atoi(argv[5]);
	else
		shared->nb_eat = UNSET;
	if (verify_args(shared) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	main(int argc, char** argv)
{
	t_shared	shared;
	t_diner		diner;

	if (argc < 5 || argc > 6)
		return (print_error(INPUT_ERR));
	if (process_args(argc, argv, &shared) == ERROR)
		return (ERROR);
	if (setup_program(&diner, &shared) == ERROR)
		return(ERROR);
	open_diner(&diner);
	clean_diner(&diner);
	return (SUCCESS);
}
