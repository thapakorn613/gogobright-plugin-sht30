// ********************* DHTxx ********************* //

Blockly.Blocks["gogo_bright.get_sht30_temp"] = {
	init: function() {
		this.appendDummyInput()
			.appendField(Blockly.Msg.SENSOR_SHT3X_TEMP)
		this.setOutput(true, 'Number');
		this.setInputsInline(true);
		this.setPreviousStatement(false);
		this.setNextStatement(false);
		this.setColour(Blockly.Msg.COLOR_SENSOR_SHT3X)
	}
};


Blockly.Blocks["gogo_bright.get_sht30_humi"] = {
	init: function() {

		this.appendDummyInput()
			.appendField(Blockly.Msg.SENSOR_SHT3X_HUMI)
		this.setOutput(true, 'Number');
		this.setInputsInline(true);
		this.setPreviousStatement(false);
		this.setNextStatement(false);
		this.setColour(Blockly.Msg.COLOR_SENSOR_SHT3X)
	}
};