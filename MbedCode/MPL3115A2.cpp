/***********************************************************************
	-Authors: A.Weiss, 7/17/2012, changes Nathan Seidle Sept 23rd, 2013 
	 mbed migration by Jorden Luke July 15 2014
	-Description: This is a Driver for the MPL3115A2 Altimeter for the mbed 
		Nucleo F401RE.  
	-License: This code falls under Public Domian and Beerware. (If you ever meet A.Weiss buy him a beer.)
		Feel free to change and add to meet your needs.
	-Hardware Connections: This can connect to any of the I2C buses on the Nucleo.
		SEE http://mbed.org/platforms/ST-Nucleo-F401RE/
	-Dependencies: mbed.h, MPL3115A2.h 	
*/
#include "MPL3115A2.h"
#include "mbed.h"

float MPL3115A2::readAltitude()
{
  
    char data_write[3];
    char data_read[3];  

    //Wait for PDR bit, indicates we have new pressure data
    int counter = 0;
    toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading
    
    //Wait for PDR bit, indicates we have new pressure data
    counter = 0;
  
    while( (IIC_Read(STATUS) & (1<<1)) == 0)
    {
       if(++counter > 100)
		{
			status = false;
			return(-999); //Error out
        }
        wait_ms(1);
    }   
  
    //Commands to the 
    data_write[0]=OUT_P_MSB;
    // Read pressure registers
    i2c->write(MPL3115A2_ADDRESS, data_write, 1,1);
    i2c->read(MPL3115A2_ADDRESS, data_read, 3, 0);  // Address of data to get
    
    toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

  // The least significant bytes l_altitude and l_temp are 4-bit,
  // fractional values, so you must cast the calculation in (float),
  // shift the value over 4 spots to the right and divide by 16 (since 
  // there are 16 values in 4-bits). 
  float tempcsb = (data_read[LSB]>>4)/16.0;

  float altitude = (float)( (data_read[MSB] << 8) | data_read[CSB]) + tempcsb;

  return(altitude);
}

//Returns the number of feet above sea level
float MPL3115A2::readAltitudeFt()
{
  return(readAltitude() * 3.28084);
}

//Reads the current pressure in Pa
//Unit must be set in barometric pressure mode
float MPL3115A2::readPressure()
{
    char data_write[3];
    char data_read[3];  

    //Wait for PDR bit, indicates we have new pressure data
    int counter = 0;
    toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading
    
    //Wait for PDR bit, indicates we have new pressure data
    counter = 0;
  
    while( (IIC_Read(STATUS) & (1<<1)) == 0)
    {
        if(++counter > 100)
		{
			status = false;
			return(-999); //Error out
        }
        wait_ms(1);
    }   
  
    //Commands to the 
    data_write[0]=OUT_P_MSB;
    // Read pressure registers
    i2c->write(MPL3115A2_ADDRESS, data_write, 1,1);
    i2c->read(MPL3115A2_ADDRESS, data_read, 3, 0);  // Address of data to get
    
    toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

    // Pressure comes back as a left shifted 20 bit number
    long pressure_whole = (long)data_read[MSB]<<16 | (long)data_read[CSB]<<8 | (long)data_read[LSB];
    pressure_whole >>= 6; //Pressure is an 18 bit number with 2 bits of decimal. Get rid of decimal portion.

    data_read[LSB] &= 0x30; //Bits 5/4 represent the fractional component
    data_read[LSB] >>= 4; //Get it right aligned
    float pressure_decimal = static_cast<float>(data_read[LSB])/4.0; //Turn it into fraction

    float pressure = (float)pressure_whole + pressure_decimal;

    return(pressure);
}

float MPL3115A2::readTemp()
{
    char data_write[3];
    char data_read[3];
    
    toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

    //Wait for TDR bit, indicates we have new temp data
    int counter = 0;
    while( (IIC_Read(STATUS) & (1<<1)) == 0)
    {
        if(++counter > 100)
		{
			status = false;
			return(-999); //Error out
        }
		wait_ms(1);
    }
  
    // Read temperature registers
    data_write[0] = OUT_T_MSB;
    i2c->write(MPL3115A2_ADDRESS, data_write,1,1);
    i2c->read(MPL3115A2_ADDRESS,data_read,3,0);  // Address of data to get
  
    // The least significant bytes l_altitude and l_temp are 4-bit,
    // fractional values, so you must cast the calculation in (float),
    // shift the value over 4 spots to the right and divide by 16 (since 
    // there are 16 values in 4-bits). 
    float templsb = (data_read[LSB]>>4)/16.0; //temp, fraction of a degree
    float temperature = (float)(data_read[MSB] + templsb);

    return(temperature);
}

//Give me temperature in Fahrenheit!
float MPL3115A2::readTempF()
{
  return((readTemp() * 9.0)/ 5.0 + 32.0); // Convert Celsius to Fahrenheit
}

//Sets the mode to Barometer
//CTRL_REG1, ALT bit
void  MPL3115A2::setModeBarometer()
{
  char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting &= ~(1<<7); //Clear ALT bit
  IIC_Write(CTRL_REG1, tempSetting);
}

//Sets the mode to Altimeter
//CTRL_REG1, ALT bit
void MPL3115A2::setModeAltimeter()
{
  char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting |= (1<<7); //Set ALT bit
  IIC_Write(CTRL_REG1, tempSetting);
}

//Puts the sensor in standby mode
//This is needed so that we can modify the major control registers
void MPL3115A2::setModeStandby()
{
  char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting &= ~(1<<0); //Clear SBYB bit for Standby mode
  IIC_Write(CTRL_REG1, tempSetting);
}

//Puts the sensor in active mode
//This is needed so that we can modify the major control registers
void MPL3115A2::setModeActive()
{
  char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting |= (1<<0); //Set SBYB bit for Active mode
  IIC_Write(CTRL_REG1, tempSetting);
}

//Setup FIFO mode to one of three modes. See page 26, table 31
//From user jr4284
void MPL3115A2::setFIFOMode(char f_Mode)
{
    if (f_Mode > 3) f_Mode = 3; // FIFO value cannot exceed 3.
    f_Mode <<= 6; // Shift FIFO byte left 6 to put it in bits 6, 7.

    char tempSetting = IIC_Read(F_SETUP); //Read current settings
    tempSetting &= ~(3<<6); // clear bits 6, 7
    tempSetting |= f_Mode; //Mask in new FIFO bits
    IIC_Write(F_SETUP, tempSetting);
}

//Call with a rate from 0 to 7. See page 33 for table of ratios.
//Sets the over sample rate. Datasheet calls for 128 but you can set it 
//from 1 to 128 samples. The higher the oversample rate the greater
//the time between data samples.
void MPL3115A2::setOversampleRate(char sampleRate)
{
    if(sampleRate > 7) sampleRate = 7; //OS cannot be larger than 0b.0111
    sampleRate <<= 3; //Align it for the CTRL_REG1 register

    char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
    tempSetting &= 0xc7; //0b11000111; //Clear out old OS bits
    tempSetting |= sampleRate; //Mask in new OS bits
    IIC_Write(CTRL_REG1, tempSetting);
}

//Clears then sets the OST bit which causes the sensor to immediately take another reading
//Needed to sample faster than 1Hz
void MPL3115A2::toggleOneShot(void)
{
    char tempSetting = IIC_Read(CTRL_REG1); //Read current settings
    tempSetting &= ~(1<<1); //Clear OST bit
    IIC_Write(CTRL_REG1, tempSetting);

    tempSetting = IIC_Read(CTRL_REG1); //Read current settings to be safe
    tempSetting |= (1<<1); //Set OST bit
    IIC_Write(CTRL_REG1, tempSetting);
}

//Enables the pressure and temp measurement event flags so that we can
//test against them. This is recommended in datasheet during setup.
void MPL3115A2::enableEventFlags()
{
    IIC_Write(PT_DATA_CFG, 0x07); // Enable all three pressure and temp event flags 
}

// These are the two I2C functions in this sketch.
char MPL3115A2::IIC_Read(char regAddr)
{
    char data_read[3];
    char data_write[3];
    
    // This function reads one byte over IIC
    data_write[0] =regAddr;
    i2c->write(MPL3115A2_ADDRESS,data_write,1,1);  // Address of CTRL_REG1
    i2c->read(MPL3115A2_ADDRESS,data_read,1,0); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
    return data_read[0];
}

void MPL3115A2::IIC_Write(char regAddr, char value)
{
    char data_write[2];
    data_write[0] = regAddr;
    data_write[1] = value;
    
    // This function writes one byte over IIC
    i2c->write(MPL3115A2_ADDRESS,data_write,2,1);
}
bool MPL3115A2::get_status()
{
	return status;
}
bool  MPL3115A2::check_connection()
{
	int counter = 0;
    while( (IIC_Read(STATUS) & (1<<1)) == 0)
    {
        if(++counter > 100)
		{
			status = false;
			return(false); //Error out
        }
		wait_ms(1);
    }

}