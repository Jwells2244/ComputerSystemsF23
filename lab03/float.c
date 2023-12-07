#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
//Used this typedef and heavily modified the print_float_bits function from:
//https://gist.github.com/mhcoma/3afe0f4a990e66366a9fa28d73b7e440, by user mhcoma.
typedef union v32_union {
	float f;
	uint32_t u;
} v32;

double pow(double n, double m){
	if (m == 0) {
        	return 1;
    	}
       	else if (m > 0) {
        	return n * pow(n, m - 1);
    	} 
	else {
        	return 1 / (n * pow(n, -m - 1));
    	}
}

void reverse(char* str){
	int length = strlen(str);
	int temp;

	for (int i = 0; i< length/2; i++){
		temp = str[i];
		str[i] = str[length-i-1];
		str[length-i-1] = temp;
	}
}
int specialCasesB(char** argv){
        char binaryStr[32];
        strncpy(binaryStr, argv[2], 32);//0 case
        if (strcmp(binaryStr, "00000000000000000000000000000000") == 0){
                printf("Binary: 00000000000000000000000000000000\n");
                printf("Sign: 0\n");
                printf("Exponent: 0\n");
                printf("Mantissa: 0\n");
                printf("Value: 0");
                return 1;
        }//inf case
	else if (strcmp(binaryStr, "01111111100000000000000000000000") == 0){
		printf("Binary: 01111111100000000000000000000000\n");
                printf("Sign: 0\n");
                printf("Exponent: 128\n");
                printf("Mantissa: 0\n");
                printf("Value: inf");
                return 1;
	}//nan case
	else if (strcmp(binaryStr, "01111111110000000000000000000000") == 0){
                printf("Binary: 01111111110000000000000000000000\n");
                printf("Sign: 0\n");
                printf("Exponent: 128\n");
                printf("Mantissa: 1.5\n");
                printf("Value: nan");
                return 1;
        }
        else{
                return 0;
        }
}




void binaryMode(char** argv){
	int specialCaseCheck = specialCasesB(argv);
	if (specialCaseCheck != 1){
		char exponent[8];
		strncpy(exponent, argv[2]+(2-1),8);
		float value;
		float mantissa = 1.0f;
		int exponentValue = strtol(exponent, NULL, 2) -127;
		for (int i = 9; i < 32; i++){
			mantissa += (argv[2][i] - '0') / (float)(1 << (i - 8));
		}
		if (exponentValue == -127){
			exponentValue = -126;
			mantissa = mantissa-1.0;
		}
		int sign = argv[2][0] - '0';
		value = (sign ? -1.0f : 1.0f) * mantissa * pow(2, exponentValue);
		printf("Binary: %s\n", argv[2]);
		printf("Sign: %c\n", argv[2][0]);
		printf("Exponent: %d\n", exponentValue);
	        printf("Mantissa: %.17g\n", mantissa);
		printf("Value: %.7g", value);
	}
		
}


void sign_exponent (float f) {
	v32 v; v.f = f;
	uint32_t mask = 1 << 31;
	int counter = 0;
	char exponent[8];
	printf("Sign: ");
	do {
		if (counter == 0){
			putchar(v.u & mask ? '1' : '0');
			printf("\nExponent: ");
			counter++;
		}
		else if (counter > 0 && counter < 9){
			exponent[counter-1] = (v.u & mask ? '1' : '0');
			counter++;
		}
	} while(mask >>= 1);
	int exponentVal = strtol(exponent, NULL, 2);
	if (exponentVal == 0){
		exponentVal = exponentVal -126;
	}
	else{
		exponentVal = exponentVal -127;
	}
	printf("%d\n", exponentVal);
}
int specialCasesF(float numVal){
        if (numVal == 0){
                printf("Binary: 00000000000000000000000000000000\n");
                printf("Sign: 0\n");
                printf("Exponent: 0\n");
                printf("Mantissa: 0\n");
                printf("Value: 0");
                return 1;
        }//inf case
        else if (isinf(numVal)){
                printf("Binary: 01111111100000000000000000000000\n");
                printf("Sign: 0\n");
                printf("Exponent: 128\n");
                printf("Mantissa: 0\n");
                printf("Value: inf");
                return 1;
        }
	else{
		return 0;
	}
} 

void floatMode(char** argv){
	float numVal = strtof(argv[2], NULL);
	int retResult = specialCasesF(numVal);
	if (retResult == 0){	
	char binaryString[32];
	v32 v; v.f = numVal;
        uint32_t mask = 1 << 31;
        printf("Binary: ");
        int counter = 0;
        do {
                binaryString[counter] = (v.u & mask ? '1' : '0');
                counter ++;
        } while (mask >>= 1);
	binaryString[32] = '\0';
	printf("%s\n", binaryString);
	char exponent[8];
	strncpy(exponent, binaryString+(1-1), 8);
	exponent[8] = '\0';
	float mantissa = 1.0f;
	for (int i = 9; i < 32; i++){
                mantissa += (binaryString[i] - '0') / (float)(1 << (i - 8));
        }
	if (strtol(exponent, NULL, 2) == 0){
		mantissa = mantissa - 1.0;
	}	
  	sign_exponent(numVal);
	if (isinf(numVal)){
		printf("Mantissa: 1.0\n");
	}
	else if (numVal == 0){
		printf("Mantissa: 0.0\n");
	}
	else{
		printf("Mantissa: %.17g\n", mantissa);
	}
	printf("Value: %.7g", numVal);
	}
}

int main(int argc, char** argv){
        if (argc != 3){
		printf("Error. Invalid number of arguments. Expected 3 but got: %d \n", argc);
		return(0);
	}
	char mode = *argv[1];
	if (mode == 102){
		floatMode(argv);
	}	
	else if (mode == 98){
		binaryMode(argv);
	}
	else{
		printf("Error. Invalid Mode given. Expected 98 or 102 but got: %d \n", mode);
		return(0);
	}
	return (0);


}
