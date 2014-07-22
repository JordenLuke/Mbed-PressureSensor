#include "mbed.h"
#include "MPL3115A2.h"
  
I2C i2c(I2C_SDA, I2C_SCL);
I2C i2c1(D5,D7);
 
MPL3115A2 sensor1(&i2c);
MPL3115A2 sensor2(&i2c1);
Serial xbee(PA_11,PA_12);
 
int serial_getline(char *ptr);
//returns the length of the array
int get_pressure(float *ptr);
int get_temp(float *ptr);
void hash_options(char cmd);
char menu();
int testing();
bool presSensor1 = false;
bool presSensor2 = false;
bool tempSensor = false;
int convertType = 2;
float pressFact = 1;
float tempFact = 1; 
int tempType = 0;
char pressEnd[10];
char tempEnd[10];

int main()
{
    char cmd = 0xFF;
    int counter =-1;
<<<<<<< HEAD
    cmd = xbee.getc();
	while (1) 
    {
      cmd = menu();
	  hash_options(cmd);
=======
    while (1) 
    {
       cmd = xbee.getc();
>>>>>>> parent of 11b1e87... Added some stuff for com
       
    }
    return 0;
}
// int serial_getline(char *ptr)
// {
    // char input;
    // int  len = 1;
    
    // char array[19];
     
    // while(input != '\n')
    // {
       input = xbee.getc();
        // array[len] = input;
        // len++;
     // }
    // ptr = array;
    // return len;
// }
//gets the pressure from the sensors
int get_pressure(float *ptr)
{
	float values[2];
	int len = 0;
    if(presSensor1)
	{
<<<<<<< HEAD
		values[0] = pressFact * sensor1.readPressure();
=======
		values[1] = pressFact * sensor1.get_pressure();
>>>>>>> parent of 11b1e87... Added some stuff for com
		len ++;
	}
    if(presSensor2)
	{
<<<<<<< HEAD
		values[1] = pressFact * sensor2.readPressure();
=======
		values[2] = pressFact * sensor2.get_pressure();
>>>>>>> parent of 11b1e87... Added some stuff for com
		len ++;
	}
	ptr = valaues;

	return len;
 }
 //gets tempEnd

 int get_temp(float *ptr)
 {
		float values[2];
		int len = 0;
		if(presSensor1)
		{
			if(tempType == 0)
			{
				values[0] = sensor1.readTemp();
			}
			if(tempType ==1)
			{
				values[0] = sensor1.readTempF();

			}
			if(tempType == 2)
			{
				values[0] = tempFact + sensor1.readTempF();
			}
			len ++;
		}
		if(presSensor2)
		{
			if(tempType == 0)
			{
				values[1] = sensor2.readTemp();
			}
			if(tempType ==1)
			{
				values[1] = sensor2.readTempF();

			}
			if(tempType == 2)
			{
				values[1] = tempFact + sensor2.readTempF();
			}
			len ++;
		}
		ptr = valaues;
}
//menu 
char menu()
{
	char cmd;

	xbee.printf("******** Menu ******** \n");
	xbee.printf("1: Enable/Disable Sensor 1 \n");
	xbee.printf("2: Enable/Disable Sensor 2 \n");
	xbee.printf("3: Enable/Disable Temperature \n");
	xbee.printf("4: Display in PSI \n");
	xbee.printf("5: Display in Torr \n");
	xbee.printf("6: Display in kpa \n");
	xbee.printf("7: Display Temperature in Celsius \n");
	xbee.printf("8: Display Temperature in Fahrenheit \n");
	xbee.printf("9:	Display Temperature in Kelvin \n");
	xbee.printf("S: Start Testing \n" );
	xbee.printf("CMD: ");
	cmd = xbee.getc();

}
//used to hash options
void hash_options(char cmd)
{
	switch(cmd)
	{
		case '1': 
			if(presSensor1)
			{
				presSensor1 = false;
				xbee.printf("Sensor 1 Disabled \n");
			}
			else
			{
				presSensor1 = true;
				xbee.printf("Sensor 1 Enabled \n");
			}
			break;
		case '2':
			if(presSensor2)
			{
				presSensor2 = false;
				xbee.printf("Sensor 2 Disabled \n");
			}
			else
			{
				presSensor2 = true;
				xbee.printf("Sensor 2 Enabled \n");
			}
			break;
		case '3':
			if(tempSensor)
			{
				tempSensor = false;
				xbee.printf("Sensor 2 Disabled \n");
			}
			else
			{
				tempSensor = true;
				xbee.printf("Sensor 2 Enabled \n");
			}
			break;
		case '4':	
			convertType = 0; 
			pressFact = 0.00014503773 ;
			break;
		case '5':
			convertType = 1;
			pressFact = 7.50061683;
			break;
		case '6':
			convertType = 2;
			pressFact = 1;
			break;
		case '7':
			tempType = 0;
			tempFact =1;
			break;
		case '8':
			tempType = 1;
			break;
		case '9':
			tempType = 2;
			tempFact = 273.15;
			break;
		case 'S':
		case 's':
			testing();
			break;
		default :
			xbee.printf("Invalid option");
	}
}
//used for the testing 
int testing()
{
	float pressure[2];
<<<<<<< HEAD
	float temp[2};
=======
>>>>>>> parent of 11b1e87... Added some stuff for com
	int len;
	xbee.printf("Press any key to start test \n");
	xbee.getc();
	if(sensor1)
	{
		xbee.printf("Sensor 1");
		switch(convertType)
		{
			case 0:
				xbee.printf(" PSI,");
				break;
			case 1:
				xbee.printf(" Torr,");
				break;
			case 2:
				xbee.printf(" Kpa,");
				break;
		}
	}
	if(tempSensor&&sensor1)
	{
		xbee.printf("Temp 1");
		switch(convertType)
		{
			case 0:
				xbee.printf(" C,");
				break;
			case 1:
				xbee.printf(" F,");
				break;
			case 2:
				xbee.printf(" K,");
				break;
		}
	}
	if(sensor2)
	{
		xbee.printf("Sensor 2");
		switch(convertType)
		{
			case 0:
				xbee.printf(" PSI,");
				break;
			case 1:
				xbee.printf(" Torr,");
				break;
			case 2:
				xbee.printf(" Kpa,");
				break;
		}
	}
	if(tempSensor&&sensor2)
	{
		xbee.printf("Temp 2")
		switch(convertType)
		{
			case 0:
				xbee.printf(" C,");
				break;
			case 1:
				xbee.printf(" F,");
				break;
			case 2:
				xbee.printf(" K,");
				break;
		}
	}
	xbee.putc('\n')

	if(!sensor1 && !sensor2)
	{
		xbee.printf("Error No Sensors Enabled");
		return -1;
	}
<<<<<<< HEAD
	while(cmd !='q' || cmd !='Q')
	{
		while(!xbee.readable())
		{
			len = get_pressure(&pressure);
			get_temp(&temp);
			if(sensor1)
			{
				xbee.printf("%3.3e,", pressure[0]);
				if(tempSensor)
				{
					xbee.printf("3.3f,",temp[0]);
				}
			}
			if(sensor2)
			{
				xbee.printf("%3.3e,", pressure[1]);
				if(tempSensor)
				{
					xbee.printf("3.3f,",temp[0]);
				}
			}
			xbee.putc('\n');
		}
		cmd = xbee.getc();
	}	
	xbee.printf("Done With Test");
	return 1;
=======
	
	if(
>>>>>>> parent of 11b1e87... Added some stuff for com
}