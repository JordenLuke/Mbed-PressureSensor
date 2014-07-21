#ifndef MPL3115A2_H
#define MPL3115A2_H

#include "mbed.h"

//MPL3115A2 REGS
#define STATUS     0x00
#define OUT_P_MSB  0x01
#define OUT_P_CSB  0x02
#define OUT_P_LSB  0x03
#define OUT_T_MSB  0x04
#define OUT_T_LSB  0x05
#define DR_STATUS  0x06
#define OUT_P_DELTA_MSB  0x07
#define OUT_P_DELTA_CSB  0x08
#define OUT_P_DELTA_LSB  0x09
#define OUT_T_DELTA_MSB  0x0A
#define OUT_T_DELTA_LSB  0x0B
#define WHO_AM_I   0x0C
#define F_STATUS   0x0D
#define F_DATA     0x0E
#define F_SETUP    0x0F
#define TIME_DLY   0x10
#define SYSMOD     0x11
#define INT_SOURCE 0x12
#define PT_DATA_CFG 0x13
#define BAR_IN_MSB 0x14
#define BAR_IN_LSB 0x15
#define P_TGT_MSB  0x16
#define P_TGT_LSB  0x17
#define T_TGT      0x18
#define P_WND_MSB  0x19
#define P_WND_LSB  0x1A
#define T_WND      0x1B
#define P_MIN_MSB  0x1C
#define P_MIN_CSB  0x1D
#define P_MIN_LSB  0x1E
#define T_MIN_MSB  0x1F
#define T_MIN_LSB  0x20
#define P_MAX_MSB  0x21
#define P_MAX_CSB  0x22
#define P_MAX_LSB  0x23
#define T_MAX_MSB  0x24
#define T_MAX_LSB  0x25
#define CTRL_REG1  0x26
#define CTRL_REG2  0x27
#define CTRL_REG3  0x28
#define CTRL_REG4  0x29
#define CTRL_REG5  0x2A
#define OFF_P      0x2B
#define OFF_T      0x2C
#define OFF_H      0x2D
#define MSB        0x00
#define CSB        0x01
#define LSB        0x02 

#define MPL3115A2_ADDRESS 0xC0 // 7-bit I2C address


class MPL3115A2
{

public:

	MPL3115A2(I2C *i2c_ref)
	{
			i2c = i2c_ref;
	}
	//retrieves altitude in Meters 
	float readAltitude();
	//retrieves altitude in Feet
	float readAltitudeFt();
	//retrieves pressures
	float readPressure();
	//retrieves temp In Celsius 
	float readTemp();
	//retrieves temp in Fahrenheit 
	float readTempF();
	//Used for Barometer mode
	void setModeBarometer();
	//Used for Altimeter
	void setModeAltimeter();
	//Enables interrupts from sensor
	void enableEventFlags();
	//Used for signal shots 
	void toggleOneShot();
	//used to change sample rate
	void setOversampleRate(char sampleRate);
	//used to set-up FIFOMode  
	void setFIFOMode(char f_Mode);
	// takes out of low power mode 
	void setModeActive();
	//puts in low power mode
	void setModeStandby();
	//gets the status
	bool get_status();
	//check connection
	bool check_connection();
	
private:
	I2C *i2c;
	bool status;
	char IIC_Read(char regAddr);
	void IIC_Write(char regAddr, char value);
	
	
};

#endif