/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/01 22:19:00 by marvin            #+#    #+#             */
/*   Updated: 2022/01/01 22:19:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_putnbr_fd(int n, int fd)
{
	int	c;

	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	write(fd, &c, 1);
}

static void get_message(int sig, siginfo_t *info, void *context)
{
	static int	len;
	static int	byte;
	int			bit;

	(void)context;
	bit = (sig == SIGUSR1);
	byte = byte << 1;
	byte = bit + byte;
	if (++len == 7)
	{
		if (!byte)
		{
			write(1, "\n\n", 2);
			kill(info->si_pid, SIGUSR2);
		}
		write(1, &byte, 1);
		len = 0;
		byte = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int main()
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_sigaction = get_message;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	write(1, "\nPID: ", 6);
	ft_putnbr_fd(getpid(), 1);
	write(1,"\n", 2);
	while (1)
		pause();
	return (0);
}