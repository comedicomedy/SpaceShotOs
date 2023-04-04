#include <Adafruit_BMP280.h>

float getAltitude(float altitude, float pressure, float temp){
  temp = cToF(temp) + 459.67;

  altitude = pressure * 10;
  altitude = altitude/101325;
  altitude = log(altitude);
  altitude = altitude * 287.053;
  altitude = altitude * temp * 5/9;
}

float cToF(float temp){
 int quotient = 9/5;
 
 temp = temp * quotient;
 temp = temp + 32;

 return temp;
}