typedef struct _foo{
	int arg1;
	int arg2;
	char arg3;
	float arg4;
} foo_t;

void* fn(void* arg){
	foo_t args = *(foo_t*)arg;
	int result;
	int* retVal;

	
	result = worker(args);
	retVal = malloc(sizeof(int));
	*retVal = result;
	return (void*)retVal;
}
int worker(foo_t arg){
	//does all the work
}


int main(){
	thread_t thr[NTHREADS];
	foo_t args[NTHREADS];

	args.arg1 = 5;
	args.arg2 = 12;
	args.arg3 = 42;
	args.arg4 = 1.3;
	for (int i = 0; i < NTHREADS; i++){
		args[i].arg1 = i;
		pthread_create(&thr[i], NULL, &((void*)args[i]));
	}
	for (int i = 0; i < NTHREADS; i++){
		pthread_join(&thr[i], &(void*)retVal);
		printf("%d\n", *retVal);//postprocess(retVal);
		free(retVal);

	}
}
