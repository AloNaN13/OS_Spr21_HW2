/*
 * notes - hw2
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}


// Process Weight

asmlinkage long sys_set_weight(int weight){
	if(weight < 0){
		return -EINVAL;
	}
	current->weight_of_task = weight;
	return 0;
}

asmlinkage long sys_get_weight(void){
	return current->weight_of_task;
}

static asmlinkage long rec_calculate_sum(struct task_struct* ts){
    struct list_head* lh;
    struct task_struct* child_of_ts;
    long total_sum = ts->weight_of_task;
    list_for_each(lh, &(ts->children)){
        child_of_ts = list_entry(lh, struct task_struct, sibling);
        total_sum = total_sum + rec_calculate_sum(child_of_ts);
    }
    return total_sum;
}

asmlinkage long sys_get_children_sum(void){
	if(list_empty(&(current->children)) !=0){
		return -ECHILD;
	}
	long sum_with_current_weight = rec_calculate_sum(current);
	return sum_with_current_weight-(current->weight_of_task);
}

asmlinkage pid_t sys_get_heaviest_ancestor(void){
    struct task_struct* process = current;
    int heaviest_weight = process->weight_of_task;
    pid_t heaviest_pid = process->pid;
    while(process->pid != 1){
        if(process->weight_of_task > heaviest_weight){
            heaviest_weight = process->weight_of_task;
            heaviest_pid = process->pid;
        }
        process = process->real_parent;
    }
	return heaviest_pid;
}
