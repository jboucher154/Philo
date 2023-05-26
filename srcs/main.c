/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:47:21 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/26 10:39:13 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	phil_startup(t_diner *diner)
{
	int					i;

	i = 0;
	diner->shared->start = get_current_time();
	while (i < diner->shared->nb_philo)
	{
		diner->all_the_phils[i]->last_meal = diner->shared->start;
		if (pthread_create(&(diner->all_the_phils[i]->phil_thread), NULL, phil_routine, diner->all_the_phils[i]))
			return (print_error("Thread creation failed"));
		i++;
	}
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
//for testing only
void	print_fork_pointers(t_diner *diner)
{
	int i;

	i = 0;
	while (i < diner->shared->nb_philo)
	{
		printf("PHILO: %d	left fork: %p	right fork: %p\n", diner->all_the_phils[i]->id, &(diner->all_the_phils[i]->left_fork), diner->all_the_phils[i]->right_fork);
		i++;
	}
	printf("----------------------------------------------------\n");
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
	print_fork_pointers(&diner);
	phil_startup(&diner);
	clean_diner(&diner);
	return (SUCCESS);
}
