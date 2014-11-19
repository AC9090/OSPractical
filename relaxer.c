#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/init.h>  

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kernel_stat.h>
#include <linux/time.h>
#include <asm/siginfo.h>
MODULE_LICENSE("GPL");

static DECLARE_WAIT_QUEUE_HEAD(WaitQueue);
static DECLARE_WAIT_QUEUE_HEAD(WaitEnd);
struct task_struct *holding[64], *p, *q;
int hold_no = 0;
int end_relaxer = 0;
int ending = 0;
bool taken[64];	
long threshold = 0.10;



static int relaxation_routine(void *irrelevant){
	
	daemonize("relaxer");
	printk("Now running relaxer\n");
	
	int irq_old = kstat_irqs(1);
	int irq_new;
	
	while (1){
	
		
		irq_new = kstat_irqs(1) + 1;
		
		if ((irq_new - irq_old) > 0){
			p = &init_task;
			q = &init_task;
			p = next_task(p);
			while (p->pid != q->pid){
				
				p = next_task(p);
				//printk("Time elapsed: %lu\nOn process: %i\n\n", cputime_to_secs(p->stime), p-> pid);
				//printk("here");
				
				if (cputime_to_secs(p->stime) > 0){
					
					//printk("%lu\n", cputime_to_secs(p->stime));
					//printk("Time elapsed: %lu\nOn process: %i\n\n", cputime_to_secs(p->stime), p-> pid);
					
					struct timespec ts;
					do_posix_clock_monotonic_gettime(&ts);
					
					long t = ts.tv_sec;
					long process_time = (t - p->start_time.tv_sec);
					
					long up_ratio = ( cputime_to_secs(p->stime) / process_time );
					printk("%lu\n", up_ratio);
					if (up_ratio >= threshold){
						//if (cputime_to_secs(p->stime)/process_time >= 0.1){
						// process meets criteria
						
						
						printk("Process with PID: %i, is ecessively busy.", p->pid);
						holding[hold_no] = p;
						taken[hold_no] = true;
						hold_no ++;
						set_current_state(TASK_INTERRUPTIBLE);
						//task_lock(p);
						//send_sig_info(SIGSTOP, p, NULL);
						//while (send_sig_info(SIGSTOP, p, NULL) < 0 ){
							//schedule();
						//}
						//task_unlock(p);
						
						if (hold_no > 63){
							printk("Relaxer: Number of excessively busy processes is > N\n");
						}
							
					}
					
					
				}
				p = next_task(p);
				
			}
		}
		irq_old = irq_new;
		set_current_state(TASK_INTERRUPTIBLE);
		
		if (end_relaxer == 1){
			ending == 1;
			wake_up(&WaitEnd);
			complete_and_exit(NULL,0);
		}
		schedule_timeout(10*HZ);
	}
	
}

static int __init init_relaxer(void){
	kernel_thread(relaxation_routine,NULL,0);
	return 0;
}

static void __exit cleanup_relaxer(void){
	end_relaxer == 1;
	wait_event(WaitEnd, ending);
}

module_init(init_relaxer);
module_exit(cleanup_relaxer);
