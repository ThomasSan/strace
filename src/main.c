int		usage(char *str)
{
	printf("usage %s <binary>\n", str);
	return (-1);
}

int		exec_child(char **argv)
{
	const char *args[] = {NULL};

	(void)argv;
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	return execv(argv[1], args)
}

int		exec_parent()
{
	unsigned int			old = 0;
	int						status;
	struct user_regs_struct	regs;

	wait(&status);
	while (1)
	{
		ptrace(PTRACE_GETREGS, child, NULL, &regs);
		if (old != regs.rip)
		{
			printf("rip : 0x%llx\n", regs.rip);
			old = regs.rip;
		}
		ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
		waitpid(child, &status, 0);
		if (WIFEXITED(status))
			break ;
	}
	return (0);
}

int		main(void)
{
	pid_t		pid;

	if (ac < 2)
		return (usage);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
		return exec_child();
	else
		return exec_parent();
	return (0);
}