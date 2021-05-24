/*
 * notes - hw2
 */

#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}


// Our Part

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
    struct tast_struct* process = current;
    int heaviest_weight = process->weight_of_task;
    pid_t heaviest_pid = process->pid;
    while(process->pid != 1){
        if(process->real_parent->weight_of_task >= process->weight_of_task){ // use parent or real_parent?
            heaviest_weight = process->real_parent->weight_of_task;
            heaviest_pid = process->real_parent->pid;
        }
        process = process->real_parent;
    }
	return heaviest_pid;
}


// Alon's Questions
    // why use asmlinkage?































// other - DELETE THIS


/*

int _internal_get_total_weight(struct task_struct* tsk){ // do we need asmlinkage?

    int total = tsk->task_weight;
    struct list_head* list;
    list_for_each(list, &tsk->children) {
        struct task_struct* child = list_entry(list, struct task_struct, sibling);
        total += _internal_get_total_weight(child);
    }
    return total;

}
	
asmlinkage long sys_get_total_weight(void){
    return _internal_get_total_weight(current);
}


asmlinkage long sys_get_heaviest_child(void){  
    int curr_max = -1;
    pid_t curr_pid = -1;
    struct list_head* list;
    list_for_each(list, &current->children) {
        struct task_struct* child = list_entry(list, struct task_struct, sibling);
        if(curr_max < _internal_get_total_weight(child)){
            curr_max = _internal_get_total_weight(child);
            curr_pid = child->pid;
        }
    }
    return (long)((curr_max >= 0)? curr_pid : -ECHILD);
}


*/