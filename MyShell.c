#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void int_handler(){
	printf("MyShell>");
}

sigaddset (&block_mask, SIGINT);

struct sigaction new_action;
new_action.sa_handler = int_handler;
new_action.sa_mask = block_mask;

sigaction(SIGINT, new_action);

char s[64];
void main(){
	while(1 == 1 ){
		printf ("MyShell>");
		fgets (s, 64, stdin);
		if (s == "Null"){  	//replace Null with blah
			printf (s, 64);
		}
	}
}

