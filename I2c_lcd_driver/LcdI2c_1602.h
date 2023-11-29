#ifndef __LCD_1602_H__
#define __LCD_1602_H__

#include <linux/i2c.h>

struct lcd_1602 {
        struct i2c_client *client;
        unsigned char data;
};

int Lcd_I2c_send_cmd(struct lcd_1602 *);
int Lcd_I2c_send_data(struct lcd_1602 *);
void Lcd_send_str(struct i2c_client *, char *);
void Lcd_clean(struct i2c_client *);
void Lcd_init(struct i2c_client *);
void Lcd_set_location(struct i2c_client *client, uint8_t x,uint8_t y);
void Lcd_backlight(struct i2c_client *client);
void Lcd_nobacklight(struct i2c_client *client);

#endif 