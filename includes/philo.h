/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:49:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/06/05 13:43:27 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define ERROR 1
# define SUCCESS 0
# define UNSET -2
# define ALIVE 1
# define DEAD 0
# define UNLOCK 0
# define LOCK 1
# define INPUT_ERR "Input Error: Input must be <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
Optional: [number_of_times_each_philosopher_must_eat]\n"

//for threads and mutexes
# include <pthread.h>
//for ssize_t
# include <sys/types.h> 
//for malloc, NULL macro
# include <stdlib.h>
//for printf
# include <stdio.h>
//for time of day and timeval struct
# include <sys/time.h>
//for usleep 
# include <unistd.h>

typedef struct s_shared
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_eat;
	int				full_phils;
	pthread_mutex_t	full_mutex;
	pthread_mutex_t	print_mutex;
	long long		start;
}					t_shared;

typedef struct s_phil
{
	struct s_shared	*shared;
	pthread_t		phil_thread;
	int				id;
	long long		last_meal;
	int				meals_eaten;
	pthread_mutex_t	meal_mutex;
	int				vital_sign;
	pthread_mutex_t	vitals_mutex;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
}				t_phil;

typedef struct s_diner
{
	struct s_shared	*shared;
	struct s_phil	**all_the_phils;
}					t_diner;

//setup
int			setup_program(t_diner *diner, t_shared *shared);

//cleanup
void		free_phils(t_diner *diner);
void		destroy_phil_mutexes(t_diner *diner, int to_destroy);
void		destroy_prog_mutexes(t_diner *diner);
void		clean_diner(t_diner *diner);

//phil factory
int			phil_factory(t_diner *diner);

//phil routine
void		*phil_routine(void *phil_to_cast);
int			check_vitals(t_phil *phil);

//task list
int			check_if_full(t_phil *phil);
int			pick_up_forks(t_phil *phil);
int			eat_meal(t_phil *phil);
int			drop_forks(t_phil *phil);

//vitals monitor
void		vitals_monitor(t_diner *diner);

//utilities_one
int			ft_atoi(const char *str);
int			print_error(char *err_msg);
size_t		ft_strlen(char *str);

//utilities_two
long long	get_current_time(void);
suseconds_t	get_current_time_micro(void);
void		protected_print(t_phil *phil, char *msg, int lock);
int			please_wait(int milli_to_wait, t_phil *phil);

#endif