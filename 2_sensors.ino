/*-----------------------------------------------------------------------------
 * SingleTact I2C Demo
 * 
 * Copyright (c) 2016 Pressure Profile Systems
 * Licensed under the MIT license. This file may not be copied, modified, or
 * distributed except according to those terms.
 * 
 * 
 * Demonstrates sensor input by reading I2C and display value on comm port.
 * 
 * The circuit: as described in the manual for PC interface using Arduino. 
 * 
 * To compile: Sketch --> Verify/Compile
 * To upload: Sketch --> Upload
 * 
 * For comm port monitoring: Click on Tools --> Serial Monitor
 * Remember to set the baud rate at 57600.
 * 
 * September 2016
 * ----------------------------------------------------------------------------- */


#include <Wire.h> //For I2C/SMBus

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  //TWBR = 12; //Increase i2c speed if you have Arduino MEGA2560, not suitable for Arduino UNO
  Serial.begin(57600);  // start serial for output
  Serial.flush();
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("PPS UK: SingleTact sensor value in PSI. \n(resembles PC executable display)");
  Serial.println("Refer manual for any other calculation.");
  Serial.println("----------------------------------------");  
}

void loop()
{   
    //short data1;  
    //float data2;
    byte i2cAddress = 0x06; // Slave address (SingleTact), default 0x04
    byte i2cNew2Address = 0x07;
    byte i2cNewAddress = 0x08; // New device address;
    short data = readDataFromSensor(i2cAddress);
    
    float C0=0;
    //float Force=0;
    //float surface = ((3.14*7)*7);
    //float Pression1, Pression ;  
    static float CapSensCref = 276.5;
    //data1= data-34;
    static float CapSensCoff = 0;
    Serial.print("I2C Sensor Data:");
    Serial.println(data);
     C0 = ((data * CapSensCref) / (16384 / 50)) + CapSensCoff;
     Serial.print("capacity is :");
    Serial.println(C0);
    delay(200);

   /* short data2 = readDataFromSensor(i2cNew2Address);
    Serial.print("||");
    Serial.print(data2);       
    //Serial.print("\n");
    delay(200);
    //unsigned short Zcdc = (unsigned short)( ((unsigned short) data << 8u) | ((unsigned short) 26)) & 0x3FFF ;
    //Serial.println(Zcdc);
    /*C0 = ((data1 * CapSensCref) / 16384) + CapSensCoff;
    Serial.print("the capacity is:     "); 
    Serial.print(C0);
    Serial.print("  pF.");     
    Serial.print("\n");
    // for (int i =0, i<
    data2 = data - 266 ;
    if (data2 < 0){
      Force = 0;
    }
    else{
      Force = ((data2 * 45) / 512 );
    }
    Pression1 = ((Force*1000) / surface);
    Serial.print(Pression1);
    Serial.print("  hPascal.");    
    Serial.print("\n");*/
    /*delay(200);// Change this if you are getting values too quickly 
    //changeSensorAddress(i2cAddress,i2cNewAddress);// wael
    short data1 = readDataFromSensor(i2cNewAddress);
     Serial.print("||");
    Serial.print(data1);       
    Serial.print("\n");
    delay(200);*/
   
  
}


short readDataFromSensor(short address)
{
  byte i2cPacketLength = 6;//i2c packet length. Just need 6 bytes from each slave
  byte outgoingI2CBuffer[3];//outgoing array buffer
  byte incomingI2CBuffer[6];//incoming array buffer

  outgoingI2CBuffer[0] = 0x01;//I2c read command
  outgoingI2CBuffer[1] = 128;//Slave data offset
  outgoingI2CBuffer[2] = i2cPacketLength;//require 6 bytes

  Wire.beginTransmission(address); // transmit to device 
  Wire.write(outgoingI2CBuffer, 3);// send out command
  byte error = Wire.endTransmission(); // stop transmitting and check slave status
  if (error != 0) return -1; //if slave not exists or has error, return -1
  Wire.requestFrom(address, i2cPacketLength);//require 6 bytes from slave

  byte incomeCount = 0;
  while (incomeCount < i2cPacketLength)    // slave may send less than requested
  {
    if (Wire.available())
    {
      incomingI2CBuffer[incomeCount] = Wire.read(); // receive a byte as character
      incomeCount++;
    }
    else
    {
      delayMicroseconds(10); //Wait 10us 
    }
  }

  short rawData = (incomingI2CBuffer[4] << 8) + incomingI2CBuffer[5]; //get the raw data

  return rawData;
}/*void changeSensorAddress(short address, short newAddress)
{
  byte i2cPacketLength = 6;//i2c packet length. Just need 6 bytes from each slave
  byte outgoingI2CBuffer[i2cPacketLength];//outgoing array buffer
  outgoingI2CBuffer[0] = 0x02;
  outgoingI2CBuffer[1] = 0x00;
  outgoingI2CBuffer[2] = 0x01;
  outgoingI2CBuffer[3] = newAddress;
  outgoingI2CBuffer[4] = 0xFF;

  /*byte i2cPacketLength = 6;//i2c packet length. Just need 6 bytes from each slave
  byte outgoingI2CBuffer[3];//outgoing array buffer
  byte incomingI2CBuffer[6];//incoming array buffer

  outgoingI2CBuffer[0] = 0x01;//I2c read command
  outgoingI2CBuffer[1] = 128;//Slave data offset
  outgoingI2CBuffer[2] = i2cPacketLength;//require 6 bytes*/
  
  /*Wire.beginTransmission(address); // transmit to device existing address
                              // device address is specified in datasheet
  Wire.write(outgoingI2CBuffer, i2cPacketLength);            // sends value byte  
  Wire.endTransmission();     // stop transmitting
}*/
