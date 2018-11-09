// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// http://store.ncd.io/product/mg-811-co2-gas-s…th-i2c-interface/

#include <Wire.h>

// ADC121C021 I2C address is 0x50(80)
#define Addr 0x50
#define         ZERO_POINT_VOLTAGE (0.366)//     (0.379)     (0.390) 
#define         REACTION_VOLTGAE             (0.03)
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))}; 
#define         DC_GAIN                      (8.5)  


int  samples = 100;
float read_volt;
void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

  float read_MQ811()
  {
    int i;
    float read_adc = 0;
   for (i=0;i<samples;i++) 
   { 
   unsigned int data[2];
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Calling conversion result register, 0x00(0)
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  
  // Convert the data to 12 bits
  read_adc +=  ((data[0] & 0x0F) * 256) + data[1];
  delay(10);
   }
 //   Serial.println(read_adc/samples);
   read_volt = (read_adc/samples) *5.00/4096;
 
  Serial.println("\n");
   return read_volt;
   return read_adc;
  }

  int MGP(float volts, float *pcurve)
  {
    if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else { 
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
  }
  
void loop()
{
  float volts;
  int percentage;
 volts = read_MQ811();
// Serial.println(volts);
//  // Output data to serial monitor
    percentage = MGP(volts,CO2Curve);
    Serial.print(" CO2 concentration in ppm : ");
    if (percentage == -1) {
        Serial.print( "<400" );
    } else {
      Serial.print(percentage);
    }

    Serial.print( "ppm" );  
    Serial.print("\n");
}
