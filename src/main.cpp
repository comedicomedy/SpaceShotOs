#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <SerialFlash.h>
#include <SD.h>
#include <GetAltitude.hpp>
#include <string.h>


Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;
const int flashCS = 5;
const int SDCS = 6;
File baromData;
SerialFlashFile flashBaromData;
File acelData;
SerialFlashFile flashAcelData;
File gyroData;
SerialFlashFile flashGyroData;
bool isStopped = false;
String altStr;

void setup() {
  pinMode(4, OUTPUT);
  pinMode(2,OUTPUT);
  Serial.begin(9600);
  //IMU settings
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);


  //Tests for error
  if (!SD.begin(SDCS)) {
    while (true   )
    {
      Serial.print("Sd Error");
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(5000);
    }
  }
  else if (!SerialFlash.begin(flashCS)) {
    while (true){
      Serial.print("Serial Flash Error");
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(1000);
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(5000);
    }
  }
  else if (!SerialFlash.begin(flashCS) && !SD.begin(SDCS)) {
    while (true){
      Serial.print("Both Error");
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(1000);
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(1000);
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(5000);
    }
  }
  else {
    while (true)
    {
      digitalWrite(4,HIGH);
    }
    
  }

  baromData = SD.open("BarometerData.txt", FILE_WRITE);
  flashBaromData = SerialFlash.open("baromData.txt");
  acelData = SD.open("AccelerometerData.txt", FILE_WRITE);
  flashAcelData = SerialFlash.open("acelData.txt");
  gyroData = SD.open("GyroscopeData.txt", FILE_WRITE);
  flashGyroData = SerialFlash.open("gyroData.txt");
}

void loop() {
  if (Serial.readString() == "stop")
  {
    isStopped = true;
  }


  if (isStopped = false && flashBaromData && baromData) {
    float altitude = getAltitude(bmp.readPressure(), bmp.readTemperature());
    altStr.concat(altitude);

    baromData.println(altStr + ", ");
    //flashBaromData.write(altStr + ", ");
    delay(500);
  }

  //if (isSt)
  //{
    
  //}
    
}
