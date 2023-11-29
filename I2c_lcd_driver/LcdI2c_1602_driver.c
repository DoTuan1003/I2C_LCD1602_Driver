#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/string.h>
#include "LcdI2c_1602.h"
struct config
{
    int set_location_x;
	int set_location_y;
    int Lcd_backlight;
};
struct config data_global;
struct lcd_1602 lcd;
int misc_open(struct inode *node, struct file *filep)
{	
	pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
	return 0;
}
int misc_release(struct inode *node, struct file *filep)
{
    pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
    return 0;
}
static ssize_t misc_read(struct file *filp, char __user *buf, size_t count,
                         loff_t *f_pos)
{   
	pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
    return 0;
}
static ssize_t misc_write(struct file *filp, const char __user *buf,
                          size_t count, loff_t *f_pos)
{

	char local_data[128];
	int ret=0;

	memset(local_data, 0, sizeof(local_data));
	ret=copy_from_user(local_data , buf , count < 128 ? count : 128);

	Lcd_send_str(lcd.client,local_data);

	pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
    return count;
}
#define MAGIC_NO1	100
#define MAGIC_NO2	101
#define MAGIC_NO3	102
#define SET_LOCATION_CMD	_IOW(MAGIC_NO1, sizeof(struct config), struct config *)
#define CLEAR_DATA_CMD	_IOW(MAGIC_NO2, sizeof(struct config), struct config *)
#define SET_BACKLIGHT_CMD	_IOW(MAGIC_NO3, sizeof(struct config), struct config *)
static long misc_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{	
	int ret=0;
    struct config local_data;
    switch(cmd)
    {
        case SET_LOCATION_CMD:
            ret=copy_from_user(&local_data,(struct config *) arg , sizeof(struct config));
            data_global.set_location_x=local_data.set_location_x;
            data_global.set_location_y=local_data.set_location_y;
			Lcd_set_location(lcd.client, data_global.set_location_x,data_global.set_location_y);
            break;
        case CLEAR_DATA_CMD:
            Lcd_clean(lcd.client);
            break;
		case SET_BACKLIGHT_CMD:
			ret=copy_from_user(&local_data,(struct config *) arg , sizeof(struct config));
			data_global.Lcd_backlight=local_data.Lcd_backlight;
			switch(data_global.Lcd_backlight)
			{
				case 0:
					Lcd_nobacklight(lcd.client);
					break;
				case 1:
					Lcd_backlight(lcd.client);
					break;
				default:
				    pr_info("invalid command\n");
           			break;
			}
			break;
        default:
            pr_info("invalid command\n");
            break;
    }
    
	pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
    return 0;
}
struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .open = misc_open, 
    .release = misc_release, 
    .read = misc_read, 
    .write = misc_write, 
    .unlocked_ioctl = misc_ioctl, 
};
static struct miscdevice misc_lcd = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "misc_lcd",
    .fops = &misc_fops,
    .mode= 0666,
};
static int lcd_1602_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	misc_register(&misc_lcd);
	lcd.client = client;
	Lcd_init(client);
	Lcd_clean(client);

	pr_info("Do Tuan:	%s\t%d\n", __func__, __LINE__);
	return 0;
}
static int lcd_1602_remove(struct i2c_client *client)
{
	misc_deregister(&misc_lcd);
	Lcd_clean(client);
	pr_info("Do Tuan:   %s\t%d\n", __func__, __LINE__);
	return 0;
}
static struct of_device_id lcd_1602_of_id[] = {
	{ .compatible = "b3,lcd1602"},
	{},
};
static struct i2c_driver lcd_1602_driver ={
	.probe	    = lcd_1602_probe,
	.remove		= lcd_1602_remove,
	.driver		= {
		.name = "lcd1602",
		.owner = THIS_MODULE,
		.of_match_table = lcd_1602_of_id,
	},
};
static int misc_init(void)
{
	return i2c_register_driver(THIS_MODULE,&lcd_1602_driver);
}

static void misc_exit(void)
{
	return i2c_del_driver(&lcd_1602_driver);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL");