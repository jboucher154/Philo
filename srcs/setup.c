/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:41:11 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/26 09:54:10 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_prog_mutexes(t_diner *diner)
{
	if (pthread_mutex_init(&(diner->shared->print_mutex), NULL) != SUCCESS)
		return (ERROR);
	if (pthread_mutex_init(&(diner->shared->full_mutex), NULL) != SUCCESS)
	{
		pthread_mutex_destroy(&(diner->shared->print_mutex));
		return (ERROR);
	}
	return (SUCCESS);
}

int	setup_program(t_diner *diner, t_shared *shared)
{
	diner->shared = shared;
	if (setup_prog_mutexes(diner) == ERROR)
		return (print_error("mutex intialization failed"));
	diner->shared->full_phils = 0;
	// diner->start = 0;
	if (phil_factory(diner) == ERROR)
	{
		destroy_prog_mutexes(diner);
		return (ERROR);
	}
	return (SUCCESS);
}
