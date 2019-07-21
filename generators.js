// ********************* DHT12 ********************* //

Blockly.JavaScript['gogo_bright.get_sht30_temp'] = function(block) {
	return [
		'DEV_I2C1.SENSOR_SHT30().readTemp()',
		Blockly.JavaScript.ORDER_ATOMIC
	];
};


Blockly.JavaScript['gogo_bright.get_sht30_humi'] = function(block) {
	return [
		'DEV_I2C1.SENSOR_SHT30().readHumi()',
		Blockly.JavaScript.ORDER_ATOMIC
	];
};