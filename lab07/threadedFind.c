#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define BUFSIZE 2048

typedef struct searchingInput{
	char word[16];
	char * path;
	int start;
	int end;
}searchingInput;

int searchInput(searchingInput args) {
	int count = 0;
	FILE * fp;
	char buffer[BUFSIZE];
	char *ret;
	char *tempRet;

	if ((fp = fopen(args.path, "rb")) == NULL) {
		printf("file failed to open");
		exit(-1);
	}
	
	fseek(fp, args.start, SEEK_SET);

	while (ftell(fp) < args.end && (fgets(buffer, BUFSIZE, fp)) != 0) {
		tempRet = buffer;
		while (tempRet) {
				ret = strstr(tempRet, args.word);
				tempRet = NULL;
				if (ret) {
					count++;
					tempRet = ret + strlen(args.word);
				}
		}
	}

	return count;
}

void *countWords(void * input) {
	searchingInput args = *(searchingInput*)input;
	int result;
	int* retval;
	
	result = searchInput(args);

	retval = malloc(sizeof(int));
	*retval = result;
	return (void*)retval;

}

unsigned int filelength(FILE* fp) {
  unsigned int size;
  fseek(fp, 0L, SEEK_END);  
  size = ftell(fp);         
  fseek(fp, 0L, SEEK_SET);  
  return size;
}


int process(char * path, char word[16], int threadCount) {
	FILE* fp;
	int sum = 0;
	unsigned int fileLength = 0;
	int cpt = 0;
	int * retval = 0;
	struct searchingInput input[threadCount];

	pthread_t thr[threadCount];
	
	if ((fp = fopen(path, "rb")) == NULL) {
		printf("file failed to open");
                exit(-1);
  	}
	
	fileLength = filelength(fp);
	cpt = fileLength/threadCount;
	fclose(fp);

	for (int i = 0; i < threadCount; i++) {
		strcpy(input[i].word, word);
		input[i].path = path;
		input[i].start = i*cpt;
		input[i].end = (i+1)*cpt;
		pthread_create(&thr[i], NULL, countWords, (void *) &(input[i]));
	}

	for (int i = 0; i < threadCount; i++) {
		pthread_join(thr[i], (void *) &retval);
		sum += *retval;
	}

	printf("%s - %d\n", path, sum);
	return 0;
}

int main(int argc, char *argv[]) {
	printf("Enter a word: ");
	char word[16];
	scanf("%s", word);

	for (int i = 2; i < argc; i++) {
		pid_t  cpid;
		cpid = fork();
		if (cpid == 0) {
			return process(argv[i], word, atoi(argv[1]));
		}
	}
	
	for (int i = 2; i < argc; i++) {
		wait(NULL);
	}
}
