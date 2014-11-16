#include <signal.h>
#include <stddef.h>

void catch_stop (){};

void main(){
	siga();
	while (1 == 1);
}
void siga(){
  struct sigaction setup_action;
  sigset_t block_mask;

  //sigemptyset (&block_mask);
  /* Block other terminal-generated signals while handler runs. */
  //sigaddset (&block_mask, SIGINT);
  sigaddset (&block_mask, SIGINT);
  setup_action.sa_handler = catch_stop;
  //setup_action.sa_mask = block_mask;
  setup_action.sa_flags = 0;
  sigaction (SIGINT, &setup_action, NULL);
}

