#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

float temp = bmp.readTemperature();

float getAltitude(float altitude){
 
  altitude = bmp.readPressure() * 10;
  altitude = altitude/101325;
  altitude = log(altitude);
  altitude = altitude * 287.053;
  altitude = cToF(temp) + 459.67 ;
}

float cToF(float templ){
 int quotient = 9/5;
 
 templ = templ * quotient;
 templ = templ + 32;

 return templ;
}