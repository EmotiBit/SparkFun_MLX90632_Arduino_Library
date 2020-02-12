/*
  Using the MLX90632 FIR Sensor

  This example shows how to remotely read the surface temperature of whatever the sensor is pointed at. 

*/

#include <Wire.h>
#include <wiring_private.h>
#include "SparkFun_MLX90632_Arduino_Library.h"
#define MOSFET 6

TwoWire myWire(&sercom1, 11, 13);
MLX90632 myTempSensor;

void setup()
{
	uint8_t address = 0x3A;
	Serial.begin(9600);
	pinMode(MOSFET, OUTPUT);
	digitalWrite(MOSFET, LOW);
	Serial.println("Enter any character to begin the Example");
	while (!Serial.available());
	Serial.println("MLX90632 Read Example");

	myWire.begin();
	pinPeripheral(11, PIO_SERCOM);
	pinPeripheral(13, PIO_SERCOM);
	myWire.setClock(1000000);

	//  myTempSensor.enableDebugging(Serial);
	MLX90632::status myStatus;
	myTempSensor.begin(address, myWire, myStatus);

	if (myStatus != MLX90632::status::SENSOR_SUCCESS)
	{
		Serial.println("MLX90632 communication failed");
		while (true);
	}

	uint16_t valueInMemory; //Create a container
	myTempSensor.readRegister16(EE_MEAS1, valueInMemory);
	Serial.print("Value stored in EE_MEAS1: 0x");
	Serial.println(valueInMemory, HEX);

	myTempSensor.readRegister16(EE_MEAS2, valueInMemory);
	Serial.print("Value stored in EE_MEAS2: 0x");
	Serial.println(valueInMemory, HEX);

	// setup digital outputs to visualize loop timing
	pinMode(14, OUTPUT);
	digitalWrite(14, LOW);
	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
	pinMode(10, OUTPUT);
	digitalWrite(10, LOW);
}

void loop()
{
	float objectTemp;

	digitalWrite(14, HIGH);
	objectTemp = myTempSensor.start_getObjectTemp(); //start measurement conversion
	digitalWrite(14, LOW);

	delay(150);  // Delay is dependent on the refresh rate set for the sensor on board
	MLX90632::status myStatus;
	myStatus = MLX90632::status::SENSOR_NO_NEW_DATA;
	digitalWrite(10, HIGH);
	while (myStatus != MLX90632::status::SENSOR_SUCCESS)
	{
		digitalWrite(16, HIGH);
		objectTemp = myTempSensor.end_getObjectTemp(myStatus); //Get the temperature of the object we're looking at in C
		digitalWrite(16, LOW);
	}
	digitalWrite(10, LOW);
	Serial.print("Object temperature: ");
	Serial.print(objectTemp, 2);
	Serial.println(" C");
}
