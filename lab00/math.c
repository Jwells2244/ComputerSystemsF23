#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	int first;
	int second;
	int result = 0;
	if (argc < 3){
		scanf("%d", &first);
		scanf("%d", &second);
	}
	else if (argc < 4){
		first = atoi(argv[2]);
		scanf("%d", &second);
	}
	else{
		first = atoi(argv[2]);
		second = atoi(argv[3]);
	}
//So the asterisk thing works if it is surrounded by quotes, otherwise I'm going to have to do some stupid implementation
	if (*argv[1] == 43){
		printf("%d", first + second);
		result = first + second;
	}
	else if (*argv[1] == 45){
		printf("%d", first - second);
		result = first - second;
	}
	else if (*argv[1]==42){
		printf("%d", first * second);
		result = first * second;
	}
	else if (*argv[1] == 47){
		if (second == 0){
			printf("0");
		}
		else{
			printf("%d", first/second);
			result = first / second;
		}
	}
	return(result);

}
