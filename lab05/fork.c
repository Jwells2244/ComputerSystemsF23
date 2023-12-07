#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv){
	printf("prompt$ ");
	char input;
	scanf("%c\n", &input);
	int pipefd[2];
	pid_t cpid;
	char buf[13];
	if (pipe(pipefd)==-1){
		perror("pipe");
		return(-1);
	}
	cpid = fork();
	if (cpid != 0){
		close(pipefd[0]);
		if (input == 105){
			write(pipefd[1], "/usr/bin/id\0", 11);
			wait(NULL);
			printf("Done - id\n");
		}
		else if (input == 112){
			write(pipefd[1], "/usr/bin/pwd\0", 12);
			wait(NULL);
                	printf("Done - pwd\n");
        	}
		else if (input == 100){
			write(pipefd[1], "/usr/bin/date\0", 13);
			wait(NULL);
                	printf("Done - date\n");
        	}
		else{
			printf("Error, bad input\n");
			return -1;
		}
	}
	else{
		close(pipefd[1]);
		//char inputs[13] = 
		read(pipefd[0], &buf, 13);
		if (buf[9] == 105){
			execl(buf, "id\0", NULL);
		}
		else if (buf[9] == 112){
			execl(buf, "pwd\0", NULL);
		}
		else if (buf[9] == 100){
			execl(buf, "date\0", NULL);
		}
		//exelc(buf, whatever, NULL);
		close(pipefd[0]);
		return 1;
	}
	return 0;
}
