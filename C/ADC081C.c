// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADC081C
// This code is designed to work with the ADC081C_I2CADC I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC081C_I2CADC#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, ADC081C I2C address is 0x50(80)
	ioctl(file, I2C_SLAVE, 0x50);

	// Select configuration register(0x02)
	// Automatic conversion mode enabled(0x20)
	char config[2] = {0};
	config[0] = 0x02;
	config[1] = 0x20;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x00)
	// raw_adc msb, raw_adc lsb
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data to 8-bits
		int raw_adc = (((data[0] & 0x0F) * 256) + (data[1] & 0xF0)) / 16;

		// Output data to screen
		printf("Raw value of the source : %d \n", raw_adc);
	}
}
