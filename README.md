OSPractical
===========

OS Practical

part 2: the thread is not even in the wait queue when it is told to wake up
test to see if the cleanup funtion is in the wait queue

http://www.read.cs.ucla.edu/111/lab2functions

need functions:
wait on condition: !list_empty(&WaitQ) in another wait queue 
maybe: waitqueue_active(wait_queue_head_t *q);

cleanup must wake after worker has recieved the signal to stop;
cleanup will always be added to the wait queue after the worker has recieved the signal to stop unless the worker waits until it knows that cleanup is on the wait queue

//
2 a) After rmmod is called on stuckworker.ko the worker routine prints to the kernel the "clocking off" message within it's next loop. The rmmod function does not return. This causes rmmod to hang in the terminal. Upon checking the list of loaded modules it can be seen that stuckworker is still in this list. This problem doesn't resolve and a reboot of the kernel is required to unload the module.

b) When the please_clock_off flag is set, the cleanup function waits to be woken up by the worker routine telling it that it is exiting. However the cleanup function does not sleep on the wait queue in time for it to be woken up. Because of this when the worker routine exits, it tells an empty queue to wake up it's processes. When the cleanup function eventually does get on to the wait list it has nothing to wake it up. It sits there indefinitely and in the eyes of the kernel the module is never properly removed since rmmod never completes.

c) The way to fix this is obviously to ensure that the cleanup funtion wakes up. It must ensure that the worker routine exited properly so it still has to wait on the worker routine to wake it up. One cannot move the flag after the 12 second timeout or change the length of the timeout since this is what simulates the race condtion between the different CPUs.

One soulution: worker_routine could make the check if both the please_clock_off flag is true and if the WaitQ queue is populated. This however doesn't really seem to be a good general solutoion and if some other process populates the queue then the problem may persist.

wait_event solution: the cleanup has a conditional flag which is set true if the worker routine is in the cleanup stage. the cleanup will not enter wait if the event has already happened and if it hasn't it will wait untill it does and worker routine will wake it.

Useful links:
https://www.kernel.org/doc/htmldocs/device-drivers/ch01s03.html

https://www.kernel.org/doc/htmldocs/device-drivers/ch01s04.html


https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/kernel/wait.c?id=refs/tags/v2.6.32.63  
-- contains code for adding to wait queue (and some info about smp)
 
 
 Part 3:
 
 interupts since boot: kstat_irqs(1) defined in linux/kernel_stat.h
 
 http://stackoverflow.com/questions/7973992/finding-amount-of-cpu-time-process-spent-in-kernel-routines --reference
 
 https://www.kernel.org/doc/htmldocs/device-drivers/API-schedule-timeout.html
 

 
The tasklist_lock protects the linked list of processes.

For SMP safe: 
Don't wait queue a process which is running on another processor.
Wake process only when not running!

http://www.linuxjournal.com/article/8144

