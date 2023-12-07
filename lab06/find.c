#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFF 2048


int main(int argc, char** argv){
	char input[20];
	printf("Enter a word: ");
        scanf("%s", input);
	pid_t cpid;
	for (int i = 1; i < argc; i++){
		cpid = fork();
		if (cpid == 0){
			FILE* fp;
			char buffer[BUFF];
			int count = 0;
			char *ret;
			char *tempRet;
			if ((fp = fopen(argv[i], "r")) == NULL){
				exit(-1);
			}
			while ((fgets(buffer, BUFF, fp))!= 0){
				tempRet = buffer;
				while (tempRet){
					ret = strstr(tempRet, input);
					tempRet = NULL;
					if (ret){
						count++;
						tempRet = ret + strlen(input);
					}
				}
			}
			fclose(fp);
			printf("%s - %d\n", argv[i], count);
			exit(0);
		}
     
	}
	if (cpid != 0){
		for (int i = 1; i < argc; i++){
			int status;
			wait(&status);
		}
        }
	return 0;
}
