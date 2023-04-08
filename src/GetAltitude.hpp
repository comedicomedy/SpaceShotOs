#include <math.h>
#include <cstdlib>

float getAltitude(float pressure, float temp){
  temp = cToF(temp) + 459.67;
  float altitude; 

  altitude = pressure * 10;
  altitude = altitude/101325;
  altitude = log(altitude);
  altitude = altitude * 287.053;
  altitude = altitude * temp * 0.566;
  altitude = altitude / -9.8;
  altitude = abs(altitude);

  return altitude;
}

float cToF(float temp){
 int quotient = 9/5;
 
 temp = temp * quotient;
 temp = temp + 32;

 return temp;
}