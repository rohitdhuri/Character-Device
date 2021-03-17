#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

static int my_open(struct inode *inode, struct file *file);
static int my_close(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *out, size_t size, loff_t *off);
static char* state_name(long s);

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
};

static struct miscdevice my_misc_device = {.minor = MISC_DYNAMIC_MINOR, .name = "process_list", .fops = &my_fops};



char **buf;
struct task_struct *task;
struct list_head *list;
int bufferIndex = 0;
int readBufferIndex = 0;

// called when module is installed
int __init init_module()
{

    misc_register(&my_misc_device);
    printk(KERN_ALERT "%s", "INSERTING MODULE\n");
    

    return 0;
}

// called when module is removed
void __exit cleanup_module()
{
    printk(KERN_ALERT "%s", "REMOVING MODULE\n");
    misc_deregister(&my_misc_device);
}

static int my_open(struct inode *inode, struct file *file)
{
    printk("MYMISCDEV: Device open\n");
    buf = kmalloc(sizeof(char*) * 4096, GFP_KERNEL);

    for_each_process(task)
    {
        buf[bufferIndex] = kmalloc(sizeof(char *)*256, GFP_KERNEL);
        sprintf(buf[bufferIndex]," PPID: %d  PID: %d  CPU: %d  STATE: %s ", task_ppid_nr(task), task->pid, task->cpu , state_name(task->state));
        //printk(KERN_INFO " PPID: %d  PID: %d  CPU: %d  STATE: %s ", task_ppid_nr(task), task->pid, task->cpu , state_name(task->state));
        bufferIndex +=1;
    }
    return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
    printk("MYMISCDIV: CLOSE DEVICE\n");
    return 0;
}

static ssize_t my_read(struct file *file, char *out, size_t size, loff_t *off)
{   
    printk("\nMYMISCDIV: READING");
    if(readBufferIndex == bufferIndex)
        return -1;
    copy_to_user(out, buf[readBufferIndex], size);
    readBufferIndex+=1;
    return 0;
}

static char* state_name(long s)
{
	switch (s)
	{
		case TASK_RUNNING: return "TASK_RUNNING";
		case TASK_INTERRUPTIBLE: return "TASK_INTERRUPTIBLE";		
		case TASK_UNINTERRUPTIBLE: return "TASK_UNINTERRUPTIBLE";
		case __TASK_STOPPED: return "__TASK_STOPPED";
		case __TASK_TRACED: return "__TASK_TRACED";
 		case EXIT_DEAD: return "EXIT_DEAD";
 		case EXIT_ZOMBIE: return "EXIT_ZOMBIE";
 		case EXIT_TRACE: return "EXIT_TRACE";
 		case TASK_PARKED: return "TASK_PARKED";
 		case TASK_DEAD: return "TASK_DEAD";
 		case TASK_WAKEKILL: return "TASK_WAKEKILL";
 		case TASK_WAKING: return "TASK_WAKING";
 		case TASK_NOLOAD: return "TASK_NOLOAD";
 		case TASK_NEW: return "TASK_NEW";
 		case TASK_STATE_MAX: return "TASK_STATE_MAX";
 		case TASK_KILLABLE: return "TASK_KILLABLE";
 		case TASK_STOPPED: return "TASK_STOPPED";
 		case TASK_TRACED: return "TASK_TRACED";
 		case TASK_IDLE: return "TASK_IDLE";
 		case TASK_NORMAL: return "TASK_NORMAL";
 		case TASK_REPORT: return "TASK_REPORT";
 		default: return "INVALID";
	}

}