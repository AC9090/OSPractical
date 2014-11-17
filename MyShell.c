#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>

void int_handler(){
	printf("\n");
}

char s[64], base[64];
char *l, *m, *arg[32];

int exit_status;

#define CH_DIR "cd"
#define EXIT "exit"

void main(){
	
	struct sigaction new_action, old_action;
	new_action.sa_handler = int_handler;
	new_action.sa_flags = 0;
	
	sigaction(SIGINT, &new_action, &old_action);
	
	while(1 == 1 ){				//main loop
		
		printf ("MyShell>");
		fgets (s, 64, stdin);
		l = s;
		int len = strlen(s);
		//printf("%s\n", len);
		if( s[len-1] == '\n' ){
   			s[len-1] = 0;
		}
		
		strcpy(base, strsep(&l , " "));
		int count = 0;
		while ((m = strsep(&l , " ")) != NULL ){
			if (strcmp(m,"") != 0){
				arg[count] = m;
				count ++;
			}
		}
		printf("%s...%s", arg[0], arg[1]);
			
		if (strcmp(base, CH_DIR) == 0){	//check for cd
			printf ("Moved to directory:\n");
					
			if (arg[0] != NULL){
				chdir(arg[0]);
			}
			printf("%s\n", getcwd());
			
		} else if (strcmp(base, EXIT) == 0){	//check for exit
			if (exit_status == 0){
				exit_status = *arg[0] - '0';
			}
			printf ("Exiting with exit status %i\n", exit_status);
			exit(exit_status);
		} else {				//run executable
			
			pid_t  pid_child;
     			pid_child = fork();
     			if (pid_child == 0){
				execvp(base, arg);
				exit(0);
			}else {
				wait(&exit_status);
			}
			
		}
	}
}

