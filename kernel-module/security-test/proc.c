#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "proc.h"
//#include "queue.h"
//#include "security_table.h"

// We're going to place all proc entries within a directory called superman.
static struct proc_dir_entry *this_proc_dir;

/*
Each proc entry consists of three parts:
	- *_proc_show - function which does the actual printf work.
	- *_proc_open - function whichs perform the file open bits.
	- *_proc_fops - structure defining the files capability.
*/


/*
Our /proc/superman/version file displays the active superman version.
*/

static int version_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "The active Superman version is %d.%d\n", SUPERMAN_VERSION_MAJOR, SUPERMAN_VERSION_MINOR);
    return 0;
}

static int version_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, version_proc_show, NULL);
}

static const struct file_operations version_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = version_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release
};

/*
Our /proc/superman/queue_info file display the queue information.
*/

/*
static int queue_info_proc_show(struct seq_file *m, void *v)
{
	int length, maxLength;
	GetQueueInfo(&length, &maxLength);
	
    	seq_printf(m, "Queue length      : %u\n"
		      "Queue max. length : %u\n", length, maxLength);

	return 0;
}

static int queue_info_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, queue_info_proc_show, NULL);
}

static const struct file_operations queue_info_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = queue_info_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release
};
*/

/*
Our /proc/superman/security_table_info file display the security table information.
*/

/*
static int security_table_info_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, security_table_info_proc_show, NULL);
}

static const struct file_operations security_table_info_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = security_table_info_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release
};
*/

/*
Our /proc/superman/root_ca file to change or display the root CA for certificate operations.
*/

//bool hasRootCA;
//void* rootCA;
struct x509_certificate* rootCA_cert;

static int root_ca_proc_show(struct seq_file *m, void *v)
{
	if(!rootCA_cert)
		seq_printf(m, "No root ca loaded.\n");
	else {
		seq_printf(m, "Root CA loaded.\n");
	}
	return 0;
}

static ssize_t root_ca_write(struct file *file, const char * buf, size_t size, loff_t * ppos)
{
//	rootCA = kmalloc(size, GFP_KERNEL);
//	if(!rootCA)
//		return -EFAULT;
//	if(copy_from_user(rootCA, buf, size)) {
//		kfree(rootCA);
//		return -EFAULT;
//	}

	if(rootCA_cert)
		x509_free_certificate(rootCA_cert);

	rootCA_cert = x509_cert_parse(buf, size);
	if(!rootCA_cert)
		return -EFAULT;
	else
		return size;
}

static int root_ca_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, root_ca_proc_show, NULL);
}

static const struct file_operations root_ca_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = root_ca_proc_open,
    .write	= root_ca_write,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release
};

/*
The proc entry init and deinit functions deal with construction and destruction.
*/

void InitProc(void)
{
	//hasRootCA = false;
	rootCA_cert = NULL;

	/* create /proc/superman */
	this_proc_dir = proc_mkdir("superman", NULL);
	if (!this_proc_dir)
		return;

	proc_create("version", 0, this_proc_dir, &version_proc_fops);
	proc_create("root_ca", 0, this_proc_dir, &root_ca_proc_fops);
//	proc_create("queue_info", 0, this_proc_dir, &queue_info_proc_fops);
//	proc_create("security_table", 0, this_proc_dir, &security_table_info_proc_fops);
}

void DeInitProc(void)
{
	if(rootCA_cert)
		x509_free_certificate(rootCA_cert);

//	remove_proc_entry("security_table", this_proc_dir);
//	remove_proc_entry("queue_info", this_proc_dir);
	remove_proc_entry("root_ca", this_proc_dir);
	remove_proc_entry("version", this_proc_dir);
	remove_proc_entry("superman", NULL);
}

