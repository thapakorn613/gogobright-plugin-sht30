#ifndef __SENSOR_SHT30_H__
#define __SENSOR_SHT30_H__

#include <string>
using std::string;

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"

#define SENSOR_SHT30_POLLING_MS 1000

#define SENSOR_SHT30_I2C_ADDRESS 0x45

//* ////////////////////////////////////
//* Register map
#define REG_GOGOBRIGHT_ID 0

class SENSOR_SHT30 : public Device
{
private:
	uint8_t _address;
	enum
	{
		s_detect,
		s_get_sensor,
		s_error,
		s_wait
	} state;

	TickType_t tickcnt, polling_tickcnt;
	bool first_read;
	bool i2c_bus_flag = 0;

	I2CDev *i2c;
	esp_err_t status_i2c;

	float cTemp;	
	float temperature=0;
	float humidity=0;
	uint8_t data_sht30[2];

	uint8_t measureCmd[2] = {0x2C,0x06};
	// uint8_t pointer_reg[6] = {0,1,2,3,4,5};

	bool wireWriteDataByte(uint8_t cmd, uint8_t param1);
	bool wireWriteDataByte(uint8_t cmd, uint8_t param1, uint8_t param2);
	bool wireWriteDataByte(uint8_t cmd, uint8_t param1, uint8_t param2, uint8_t param3);

public:
	SENSOR_SHT30();
	// override
	void init(void);
	void process(Driver *drv);
	int prop_count(void);
	bool prop_name(int index, char *name);
	bool prop_unit(int index, char *unit);
	bool prop_attr(int index, char *attr);
	bool prop_read(int index, char *value);
	bool prop_write(int index, char *value);

	//* ********************* I2C port functions *********************
	float readTemp(void);
	float readHumi(void);
};
#endif
