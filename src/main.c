#include "strace.h"

int		usage(char *str)
{
	printf("usage %s <binary>\n", str);
	return (-1);
}

void	exec_child(char **argv)
{
	char * const args[] = {NULL};

    (void)argv;
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execv("./a.out", args);
}

void	exec_parent(pid_t pid)
{
	unsigned int			old = 0;
	int						status;
	struct user_regs_struct	regs;

	wait(&status);
	while (1)
	{
		ptrace(PTRACE_GETREGS, pid, NULL, &regs);
		if (old != regs.rip)
		{
			printf("rip : 0x%llx\n", regs.rip);
			old = regs.rip;
		}
		ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			break ;
	}
}

int		main(int ac, char **av)
{
	pid_t		pid;

	if (ac < 2)
		return (usage(av[0]));
	pid = fork();
    printf("fork\n");
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
    {
        printf("child\n");
		exec_child(av+1);
    }
	else
    {
        printf("parent\n");
		exec_parent(pid);
    }
	return (0);
}
