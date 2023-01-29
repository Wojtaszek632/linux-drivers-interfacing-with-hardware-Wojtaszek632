#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <asm/ioctl.h>
#include <asm/io.h>

#include "mydriver.h"

#define MY_MAJOR 140
#define MY_MAX_MINORS 10
#define MY_FIRST_MINOR 0
#define NUMBER_LEN 32



static int my_init(void);
static void my_exit(void);

struct my_device_data {
struct cdev cdev;
/* data to record and identify the device */
ssize_t size;
char *buffer;

};

static int LastWrittenRegIndx = 0;

struct my_peripheral {
  void __iomem *regs;
};

  struct resource *res;
  struct my_peripheral *peripheral;

struct my_device_data devs[MY_MAX_MINORS];

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("K Opening the driver...\n");
	struct my_device_data *my_data = container_of(inode->i_cdev, struct my_device_data, cdev);
	
	unsigned int minor = MINOR(inode->i_cdev->dev);
	/* validate access to device */
	file->private_data = my_data;
	/* initialize device */
	pr_info("K Opening successfull\n");
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	pr_info("K Releasing the driver...\n");
	pr_info("K Releasing succesfull :D\n");
 	return 0;
}


static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	pr_info("K Reading from the driver...\n");
	
	char *data = (char*)kmalloc(11,GFP_USER);
	unsigned int RESULT_REG = 0x00;
  	RESULT_REG = ioread32(peripheral->regs + RESULT_REG_OFFSET);
  	printk("K Read value for peripherial: %d\n", RESULT_REG);
	size_t datalen = sprintf(data,"%d\n",RESULT_REG);
	if (count > datalen) {
		count = datalen;
	}
	if (copy_to_user(buf, data, count)) {return -EFAULT;}
	pr_info("K Reading successfull\n");
	kfree(data);
	return count;
	
	  
 
}

static int my_write(struct file *file, const char __user *user_buffer,size_t size, loff_t * offset)
{
	pr_info("K Writing to the driver...\n");
    
    if (size <= 0)        return 0;
    pr_info("K Recived bytes:(%d)\n",size);
	char *data = (char*)kmalloc(size+1,GFP_USER);
   
    if (copy_from_user(data, user_buffer, size)) { return -EFAULT;}
	pr_info("K RECIVED FROM USER APP:  ");
	pr_info("%s\n",data);
	
    long IntVal=0;
	if (kstrtol(data, 10, &IntVal)) { return -EFAULT;}
    
    if (LastWrittenRegIndx == 0)
    {
    iowrite32(IntVal,peripheral->regs + DAT0_REG_OFFSET);
    LastWrittenRegIndx = 1;
    }
    else if(LastWrittenRegIndx == 1)
    {
      iowrite32(IntVal,peripheral->regs + DAT1_REG_OFFSET);
      LastWrittenRegIndx = 0;
    }
    
    pr_info("K Converted to long & wrtited to peripherial\n");
    kfree(data);
    
    pr_info("K Writing succesfull\n");
    return size;
}




static long my_ioctl (struct file *file, unsigned int cmd,unsigned long arg)
{
	pr_info("K Reading IOCTL...\n");
	struct my_device_data *my_data = (struct my_device_data*) file->private_data;
	struct my_ioctl_data mid;
	
	switch(cmd) {
		case MY_IOCTL_IN:
			pr_info("K MY_IOCTL_IN...\n");
			if( copy_from_user(&mid, (my_ioctl_data *) arg, sizeof(my_ioctl_data)) )
				return -EFAULT;
		break;

		case reset:
		pr_info("K Requesting: reset\n");
		iowrite32(0, peripheral->regs + DAT0_REG_OFFSET);
		iowrite32(0, peripheral->regs + DAT1_REG_OFFSET);
		iowrite32(OPERATION_ADD, peripheral->regs + OPERATION_REG_OFFSET);
		break;

		case  operation_add:
		pr_info("K Requesting: OPERATION_ADD\n");
		iowrite32(OPERATION_ADD,peripheral->regs + OPERATION_REG_OFFSET);		
		break;
		
		case  operation_subs:
		pr_info("K Requesting: OPERATION_SUB\n");
		iowrite32(OPERATION_SUB,peripheral->regs + OPERATION_REG_OFFSET);	
		break;
		
		case  operation_mult:
		pr_info("K Requesting: OPERATION_MULT\n");
		iowrite32(OPERATION_MULT,peripheral->regs + OPERATION_REG_OFFSET);	
		break;
		
		case  operation_div:
		pr_info("K Requesting: OPERATION_DIV\n");
		iowrite32(OPERATION_DIV,peripheral->regs + OPERATION_REG_OFFSET);	
		break;
		
		case  check_status:
			pr_info("K Requesting: check_status\n");
			unsigned int STATUS_REG = 0x00;
			STATUS_REG = ioread32(peripheral->regs + STATUS_REG_OFFSET);
			printk("K check_status: %d\n", STATUS_REG);
			switch(STATUS_REG) {
				case STATUS_INVALID_OPERATION:
				pr_info("K STATUS_INVALID_OPERATION\n");
				break;
				case STATUS_DIV_BY_ZERO:
				pr_info("K STATUS_DIV_BY_ZERO\n");
				break;
				}
				
		break;
		
		case  error_ack:
		pr_info("K Requesting: error_ack\n");
		LastWrittenRegIndx = 0;
		iowrite32(ERROR_ACK,peripheral->regs + STATUS_REG_OFFSET);	
		break;
 			
		default:
			return -ENOTTY;
	}
	
	pr_info("K Reading IOCTL successfull\n");
	
		return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.release = my_release,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
};


static const struct of_device_id my_driver_dt_ids[] = {

	{ 		.compatible = "uwr,my-driver" 	},
	{	},
};

static int my_driver_probe (struct platform_device *pdev)
{
	
	pr_info("K Probing the driver...\n");
	
	int i, err;
	// register character device driver 
	err = register_chrdev_region(MKDEV(MY_MAJOR, MY_FIRST_MINOR), MY_MAX_MINORS,"my_device_driver");
	/* report error */
	if (err != 0) {	pr_info("K register_chrdev_region error\n"); return err;	}
	pr_info("K Registering succesfull\n");
	for(i = 0; i < MY_MAX_MINORS; i++) {
		/* initialize devs[i] fields */
		cdev_init(&devs[i].cdev, &my_fops);
		cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
	}
	
	


 

  

  peripheral = devm_kzalloc(&pdev->dev, sizeof(*peripheral), GFP_KERNEL);
  if (!peripheral)
    return -ENOMEM;

  res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  
  printk("K Peripherial Resource start addr: 0x%x\n", res->start);

  peripheral->regs = devm_ioremap_resource(&pdev->dev, res);
  if (IS_ERR(peripheral->regs))
    return PTR_ERR(peripheral->regs);

  
  
  

	
	
	
	pr_info("K Probing successfull\n");
	return 0;
}

static int my_driver_remove (struct platform_device *pdev)
{
	
	pr_info("K Removing the driver...\n");
	
	int i;
 	for (i = 0; i < MY_MAX_MINORS; i++) {
		/* release devs[i] fields */
		cdev_del(&devs[i].cdev);
	}
	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
	kfree(res);
	pr_info("K Removing successfull\n");
	return 0;
}

static struct platform_driver my_driver = {
	.driver = {
		.name = "my_awesome_driver" ,
		.of_match_table = my_driver_dt_ids
	 },
	.probe = my_driver_probe,
	.remove = my_driver_remove,
};

static int __init my_init()
{
	pr_info("K Registering the driver!\n" );
	
	return platform_driver_register(&my_driver);
}

static void __exit my_exit()
{
	pr_info("K Exiting, Goodbye! :)\n");
}

module_init(my_init);

module_exit(my_exit);

MODULE_LICENSE("GPL");
