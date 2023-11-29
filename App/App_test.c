#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define FILE	"/dev/misc_lcd"
#define MAGIC_NO1	100
#define MAGIC_NO2	101
#define MAGIC_NO3	102
#define SET_LOCATION_CMD	_IOW(MAGIC_NO1, sizeof(struct config), struct config *)
#define CLEAR_DATA_CMD	_IOW(MAGIC_NO2, sizeof(struct config), struct config *)
#define SET_BACKLIGHT_CMD	_IOW(MAGIC_NO3, sizeof(struct config), struct config *)
struct config
{
    int set_location_x;
	int set_location_y;
    int Lcd_backlight;
};
struct config config_data;
char data[128];

int main()
{
	int fd = -1;
	int menu = 0;
	
	while (1) {
		printf("\nPress 1 to write data to file.");
		printf("\nPress 2 to send an IOCTL set location cusor.");
		printf("\nPress 3 to send an IOCTL backlight.");
        printf("\nPress 4 to clean data.\n");
		printf("\nPress 5 to exit program.\n");
		fflush(stdin);
		scanf("%d", &menu);
		
		switch (menu) {
			case 1:
				printf("\nInput data to write: ");
				fflush(stdin);
				memset(data, 0, sizeof(data));
				scanf("%s", data);
				fd = open(FILE, O_RDWR);
				write(fd, data, strlen(data));
				close(fd);
				break;
			case 2:
            	printf("\nset location row: ");
				fflush(stdin);
                scanf("%d",&config_data.set_location_x);
                printf("\nset location column: ");
				fflush(stdin);
                scanf("%d",&config_data.set_location_y);
				fd = open(FILE, O_RDWR);
				ioctl(fd, SET_LOCATION_CMD, &config_data);
				close(fd);
				break;
			case 3:
                printf("\nPress 0 to unset blacklight,Press 1 to set blacklight: ");
				fflush(stdin);
                scanf("%d",&config_data.Lcd_backlight);
				fd = open(FILE, O_RDWR);
				ioctl(fd, SET_BACKLIGHT_CMD, &config_data);
				close(fd);
				break;
            case 4:
				fd = open(FILE, O_RDWR);
				ioctl(fd, CLEAR_DATA_CMD, &config_data);
				close(fd);
				break;
			case 5:
				return 0;
			default:
				printf("\nOperation %d is not supported.\n", menu);
				return 0;
		}
	}

	return 0;
}
