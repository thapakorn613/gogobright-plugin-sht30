#include <stdio.h>
#include <math.h>
#include <locale>
#include "esp_system.h"
#include "kidbright32.h"
#include "sensor_sht30.h"

SENSOR_SHT30::SENSOR_SHT30()
{
	channel = 0; //bus_ch;
	address = SENSOR_SHT30_I2C_ADDRESS;
	polling_ms = SENSOR_SHT30_POLLING_MS;
}

void SENSOR_SHT30::init(void)
{
	first_read = true;
	cTemp = 0;
	state = s_detect;
}

int SENSOR_SHT30::prop_count(void)
{
	return 2;
}

bool SENSOR_SHT30::prop_name(int index, char *name)
{
	if (index == 0)
	{
		snprintf(name, DEVICE_PROP_NAME_LEN_MAX, "%s", "Temperature value");
		return true;
	}
	else if (index == 1)
	{
		snprintf(name, DEVICE_PROP_NAME_LEN_MAX, "%s", "Relative Humidity value");
		return true;
	}

	// not supported
	return false;
}

bool SENSOR_SHT30::prop_unit(int index, char *unit)
{
	if (index == 0)
	{
		snprintf(unit, DEVICE_PROP_UNIT_LEN_MAX, "%s", "Celsius");
		return true;
	}
	else if (index == 1)
	{
		snprintf(name, DEVICE_PROP_UNIT_LEN_MAX, "%s", "Percent");
		return true;
	}

	// not supported
	return false;
}

bool SENSOR_SHT30::prop_attr(int index, char *attr)
{
	if (index == 0)
	{
		get_attr_str(attr, PROP_ATTR_READ_FLAG | PROP_ATTR_TYPE_NUMBER_FLAG); // read only, number
		return true;
	}

	// not supported
	return false;
}

bool SENSOR_SHT30::prop_read(int index, char *value)
{
	if (index == 0)
	{
		snprintf(value, DEVICE_PROP_VALUE_LEN_MAX, "%f", temperature);
		return true;
	}
	else if (index == 1)
	{
		snprintf(value, DEVICE_PROP_VALUE_LEN_MAX, "%f", humidity);
		return true;
	}

	return false;
}

bool SENSOR_SHT30::prop_write(int index, char *value)
{
	// not supported
	return false;
}

void SENSOR_SHT30::process(Driver *drv)
{
	// I2CDev *i2c = (I2CDev *)drv;
	i2c = (I2CDev *)drv;

	uint8_t pointer_reg;
	switch (state)
	{
	case s_detect:
		// stamp polling tickcnt
		polling_tickcnt = get_tickcnt();

		// detect i2c device
		status_i2c = i2c->detect(channel, SENSOR_SHT30_I2C_ADDRESS);

		if (status_i2c == ESP_OK)
		{
			initialized = true;
			// clear error flag
			error = false;
			// get current tickcnt
			tickcnt = get_tickcnt();

			// i2c->write(channel, SENSOR_SHT30_I2C_ADDRESS, &measureCmd[0], 1);
			// i2c->write(channel, SENSOR_SHT30_I2C_ADDRESS, &measureCmd[1], 1);

			state = s_get_sensor;
		}
		else //* try on other i2c bus
		{
			state = s_error;
			temperature = status_i2c;
		}
		break;
	case s_get_sensor:
		// read delay for 40ms
		i2c->write(channel, SENSOR_SHT30_I2C_ADDRESS, measureCmd, 2);

		if (is_tickcnt_elapsed(tickcnt, 500))
		{

			// Read 8 bytes for 4 analog sensors
			for (int i = 0; i < 2; i++)
			{
				pointer_reg = i;
				if (is_tickcnt_elapsed(tickcnt, 50))
					i2c->read(channel, address, &pointer_reg, 1, &data_sht30[i], 1);
			}

			// esp_err_t status_sht30 = i2c->read(channel, address, pointer_reg, 6, data_sht30, 6);
			// for (int i = 0; i < 5; i += 2)
			// {
			// 	byte = i;
			// 	i2c->read(channel, address, &byte, 1, &raw_data[i], 1);
			// }

			// ****** Checksum ******
			// if ((raw_data[0] + raw_data[1] + raw_data[2] + raw_data[3]) & 0xFF != raw_data[4])
			// {
			// 	state = s_error;
			// 	break;
			// }

			// ****** Calc Temp ******

			// uint8_t scaleValueTemp = raw_data[3] & 0x7F;
			// byte signValueTemp  = raw_data[3] & B10000000;
			// cTemp = 0;
			// if (status_sht30 == ESP_OK)
			// {
			temperature = (((((data_sht30[0] * 256.0) + data_sht30[1]) * 175) / 65535.0) - 45);
			// temperature = raw_data[0];
			// humidity = 67;
			// humidity = ((((raw_data[3] * 256.0) + raw_data[4]) * 100) / 65535.0);
			// temperature = (raw_data[2] + (float)raw_data[3] / 10);
			// if (signValueTemp)  // negative temperature
			//     temperature = -temperature;

			// ****** Calc Humi ******
			// humidity = (raw_data[0] + (float)raw_data[1] / 10);

			initialized = true;
			// load polling tickcnt
			tickcnt = polling_tickcnt;
			// goto wait and retry with detect state
			state = s_wait;
			// }
			// else {
			// 	state = s_error;
			// 	temperature = 66;
			// }
		}
		break;
	case s_error:
	
		// set error flag
		error = true;
		// clear initialized flag
		initialized = false;
		// set first read flag
		first_read = true;
		// get current tickcnt
		tickcnt = get_tickcnt();
		// goto wait and retry with detect state
		state = s_wait;
		// set flag to switch to comm on other i2c bus
		i2c_bus_flag = !i2c_bus_flag;
	
		break;

	case s_wait:
		// wait polling_ms timeout
		if (is_tickcnt_elapsed(tickcnt, polling_ms))
		{
			state = s_detect;
		}
		break;
	}
}

//* ************* Input port functions *************

float SENSOR_SHT30::readTemp(void)
{
	return temperature;
}

float SENSOR_SHT30::readHumi(void)
{
	return humidity;
}
