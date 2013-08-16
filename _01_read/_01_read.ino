
// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2c/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <I2C.h>
#include "const_values.h"

// Making use of the I2c library found here:
// http://www.dsscircuits.com/articles/arduino-i2c-master-library.html
// to facilitate the repeated start condition (ie register reading)

// As of 8/15/13, I'm using Rev 5 of that library

void setup()
{
  I2c.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  setup_read();
}

void loop()
{
  acceleration_3d_t poll_result = poll();
  
  Serial.print("X: ");
  Serial.print(poll_result.x, DEC);
  Serial.print("\tY: ");
  Serial.print(poll_result.y, DEC);
  Serial.print("\tZ: ");
  Serial.println(poll_result.z, DEC);
  
  delay(500);
}


void setup_read(){
  MCTL_value_t writeable;
  writeable.v.mode = MEASURE;
  writeable.v.glvl = _2G;
  writeable.v.ston = 0;
  writeable.v.drpd = 1;
  
  I2c.write(dev_addr_0, MCTL, writeable.b);
}

acceleration_3d_t poll(){
  I2c.read(dev_addr_0, (byte)0x0, (byte)6);
  acceleration_3d_t poll_result;
  int value;
  // X
  value = I2c.receive();
  value = ((I2c.receive()) << 10) + (value & 0xFF);
  poll_result.x = value;
  
  value = I2c.receive();
  value = ((I2c.receive()) << 10) + (value & 0xFF);
  poll_result.y = value;
  
  value = I2c.receive();
  value = ((I2c.receive()) << 10) + (value & 0xFF);
  poll_result.y = value;
  
  return poll_result;
}
