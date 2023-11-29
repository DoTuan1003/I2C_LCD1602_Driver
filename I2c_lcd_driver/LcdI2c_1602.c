#include <linux/delay.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include "LcdI2c_1602.h"
int Lcd_I2c_send_cmd(struct lcd_1602 *lcd)
{   
    int length=0;
	uint8_t buf_t[4]={
        (lcd->data & 0xf0)| 0x0C,
        (lcd->data & 0xf0)| 0x08,
        ((lcd->data << 4) & 0xf0)| 0x0C,
        ((lcd->data << 4) & 0xf0)| 0x08,
    };

	length = i2c_master_send(lcd->client, buf_t, 4);
	if(length != 4) {
		pr_err("Sending cmd failed\n");
		return -1;
	}
    pr_info("Sending cmd sucess\n");
	return 0;
}
int Lcd_I2c_send_data(struct lcd_1602 *lcd)
{   
    int length=0;
	uint8_t buf_t[4]={
        (lcd->data & 0xf0)| 0x0D,
        (lcd->data & 0xf0)| 0x09,
        ((lcd->data << 4) & 0xf0)| 0x0D,
        ((lcd->data << 4) & 0xf0)| 0x09,
    };

	length = i2c_master_send(lcd->client, buf_t, 4);
	if(length != 4) {
		pr_err("Sending data failed\n");
		return -1;
	}
    pr_info("Sending data sucess\n");
	return 0;
}
void Lcd_send_str(struct i2c_client *client, char *str)
{
	struct lcd_1602 lcd;
	lcd.client = client;

	while(*str) {
		lcd.data = *str;
        if(*str==0xA)
            break;
		if(Lcd_I2c_send_data(&lcd))
			break;

		str++;
	}
}
void Lcd_set_location(struct i2c_client *client, uint8_t x, uint8_t y)
{
	struct lcd_1602 lcd;
	lcd.client = client;
    
    if(x==0)
    {
    lcd.data = 0x80 + y;
    Lcd_I2c_send_cmd(&lcd);
    }
    else if (x==1)
    {
    lcd.data = 0xC0 + y;
    Lcd_I2c_send_cmd(&lcd);  
    }
    else{
        pr_err("Cmd set_location failed\n");
    }
}
void Lcd_clean(struct i2c_client *client)
{
	struct lcd_1602 lcd;
    lcd.client = client;

	lcd.data = 0x01;
	Lcd_I2c_send_cmd(&lcd);
    msleep(20);

}
void Lcd_backlight(struct i2c_client *client)
{
    struct lcd_1602 lcd;
    lcd.client = client;

	lcd.data = 0x08;
	Lcd_I2c_send_cmd(&lcd);
    msleep(20);

}
void Lcd_nobacklight(struct i2c_client *client)
{
    struct lcd_1602 lcd;
    lcd.client = client;

	lcd.data = 0x0C;
	Lcd_I2c_send_cmd(&lcd);
    msleep(20);

}
void Lcd_init(struct i2c_client *client)
{
    struct lcd_1602 lcd;
	lcd.client = client;

	lcd.data = 0x33;//lenh khoi tao
	Lcd_I2c_send_cmd(&lcd);

    lcd.data = 0x32;//lenh khoi tao
    Lcd_I2c_send_cmd(&lcd);

    lcd.data = 0x28;// che do 4 bit 2 hang
    Lcd_I2c_send_cmd(&lcd);

    lcd.data = 0x0C;// hien thi man hinh va tat con tro
    Lcd_I2c_send_cmd(&lcd);

    lcd.data = 0x06;// tang con tro
    Lcd_I2c_send_cmd(&lcd);
}
MODULE_LICENSE("GPL");