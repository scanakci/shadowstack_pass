#include <stdio.h>
#include <stdlib.h>

#define unlikely(x)     __builtin_expect((x),0)
//globals

char **SS_ptr;
int  pNum = 0;
int  eNum = 0;
unsigned long long pTotalNum = 0;
unsigned long long eTotalNum = 0;


void createSS(void) __attribute__((always_inline));
void createSS(void){

//printf("Shadow Stack Created\n");
SS_ptr = (char**) malloc(8192* sizeof(char*)); //prev 

}

void logop(unsigned long long i) {    
	printf("0x%llx\n",i);//, i);
}


void insertPC(char *prolog_PC) __attribute__((always_inline));
void insertPC(char *prolog_PC){//prolog function:insert original return address into SS

	*SS_ptr = prolog_PC;
	SS_ptr++;
//	printf("Prolog:%p\n",prolog_PC);
//	pTotalNum++;


} 

void checkPC(char *epilog_PC) __attribute__((always_inline));
void checkPC(char *epilog_PC){//epilog function:pop from SS and compare 
	char* orig_PC;
	SS_ptr--;
	orig_PC = *SS_ptr;
//	printf("Popped from SS:%p\n", orig_PC);
//	printf("Epilog:%p\n", epilog_PC);
//	eTotalNum++;
	if(unlikely(orig_PC != epilog_PC)){
		printf("total numbers %llu %llu",pTotalNum, eTotalNum);
		printf("Shadow Stack mismatch! STOP!\n");
		exit(0);	
	}
}

void logop2(char *k){//get the latest address, pop from SS and compare

    unsigned long long *i = (unsigned long long *)k;	
	printf("%p\n",k);

}
