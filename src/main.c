#include "strace.h"

int		usage(char *str)
{
	printf("usage %s <binary>\n", str);
	return (-1);
}

int     wait_for_syscall(pid_t pid)
{
    int status;
    while (1)
    {
        ptrace(PTRACE_SYSCALL, pid, NULL, 0);
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
            return (0);
        if (WIFEXITED(status))
            return (1);
    }
    return (1);
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
	int						status;
	int						syscall;
	int						retval;

    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
        if (wait_for_syscall(pid) != 0)
            break ;
        syscall = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX);
        printf("syscall(%d) = ", syscall);
        if (wait_for_syscall(pid) != 0)
            break ;
        retval = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX);
        printf("%d\n", retval);
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
