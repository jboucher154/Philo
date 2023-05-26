/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:57:06 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/25 19:48:41 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * overflowed returns -1 if the sign is positive, 0 if the sign is negative,
 * otherwise it returns 1.
*/
static int	overflowed(int sign)
{
	if (sign > 0)
		return (-1);
	else if (sign < 0)
		return (0);
	else
		return (1);
}

/*
 * ft_atoi converts a string to a integer. It returns the integer on
 * success and 0 on failure.
*/
int	ft_atoi(const char *str)
{
	int			i;
	long int	sum;
	int			sign;

	i = 0;
	sum = 0;
	sign = 1;
	if (str[i] > 57 || !str[i])
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		sum = sum * 10 + (str[i] - 48);
		i++;
	}
	if (sum < 0)
		return (overflowed(sign));
	return (sum * sign);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	print_error(char *err_msg)
{
	write(2, "Philo: Error: ", 14);
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
	return (ERROR);
}
