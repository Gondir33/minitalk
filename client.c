/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 01:56:16 by marvin            #+#    #+#             */
/*   Updated: 2022/01/02 01:56:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	the_end(int sig, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (sig == SIGUSR2)
	{
		write(1, "message delivered\n", 18);
		exit(0);
	}
}

int	ft_pid(const char *s)
{
	int	n;

	n = 0;
	while ((*s > 8 && *s < 14) || *s == 32)
		s++;
	while (*s >= '0' && *s <= '9')
	{
		n = n * 10 + *s - '0';
		s++;
	}
	return (n);
}

void	send_message(int pid, int c)
{
	int	counter;

	counter = 1 << 6;
	while (counter)
	{
		if (counter & c)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		counter >>= 1;
		usleep(1000);
	}
}

int	main(int ac, char **av)
{
	struct sigaction	act;
	int					pid;
	int					i;

	i = 0;
	if (ac != 3)
		return (-1);
	act.sa_sigaction = the_end;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	pid = ft_pid(av[1]);
	while (av[2][i])
		send_message(pid, av[2][i++]);
	send_message(pid, 0);
}
