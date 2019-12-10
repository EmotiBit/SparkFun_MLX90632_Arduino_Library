/*
  Using the MLX90632 FIR Sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14569

  This example shows how to write to an EEPROM register. This is handy if you need
  to change the REG_I2C_ADDRESS register to make the sensor be a totally different I2C address.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>
#include <wiring_private.h>

TwoWire myWire(&sercom1, 11, 13);
#include "SparkFun_MLX90632_Arduino_Library.h"
MLX90632 myTempSensor;

void setup()
{
  uint8_t address = 0x3A;
  Serial.begin(9600);
  while(!Serial.available());
  Serial.println("MLX90632 Read Example");

  myWire.begin();
  pinPeripheral(11,PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
  myWire.setClock(100000);
//  myTempSensor.enableDebugging(Serial);
  MLX90632::status tempStatus;
  myTempSensor.begin(address, myWire, tempStatus);

  //readRegister16 returns a status value not the value found at the memory location
  //We have to pass in a container for readRegister to store the data into
  uint16_t valueInMemory; //Create a container
  myTempSensor.readRegister16(EE_Hb, valueInMemory);
  Serial.print("Value stored in EE_Hb: 0x");
  Serial.println(valueInMemory, HEX);

  //Write a new dummy value to EE_Hb register.
  myTempSensor.writeEEPROM(EE_Hb, 0x02AB);

  myTempSensor.readRegister16(EE_Hb, valueInMemory);
  Serial.print("New value stored in EE_Hb (should be 0x02AB): 0x");
  Serial.println(valueInMemory, HEX);

  Serial.println("Done");
  uint16_t sta;
  myTempSensor.readRegister16(REG_CONTROL, sta);
  Serial.print("REG_CONTROL");
  sta &= 0x0006;
  Serial.println(sta, HEX);
//
//  myTempSensor.writeEEPROM(REG_CONTROL, REG_CONTROL|0x1116);
//  myTempSensor.readRegister16(REG_CONTROL, sta);
//  Serial.print("changed REG_CONTROL:");
//  sta &= 0x0006;
//  Serial.println(sta, HEX);

}

void loop()
{
  float objectTemp;

  //For the world
  objectTemp = myTempSensor.start_getObjectTemp(); //Get the temperature of the object we're looking at in C
  for (int i = 0;i<6; i++){
    Serial.println(i);
    delay(100);
  }
  objectTemp = myTempSensor.end_getObjectTemp(); //Get the temperature of the object we're looking at in C
  Serial.print("Object temperature: ");
  Serial.print(objectTemp, 2);
  Serial.println(" C");
}
