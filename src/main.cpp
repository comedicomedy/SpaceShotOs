#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <SerialFlash.h>
#include <SD.h>
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
char buf[1];
String Acel[3];
String gyroStr;

//Calculates Altitude
float getAltitude(float pressure, float temp){
  int quotient = 9/5;

  temp = temp * quotient;
  temp = temp + 32;
  
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


void setup() {
  //LED Settings
  pinMode(4, OUTPUT);
  pinMode(2,OUTPUT);
  //Starting Serial
  Serial.begin(9600);
  //IMU settings
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);


  //Tests for errors
  if (!SD.begin(SDCS)) {
    isStopped = true;

    while (true)
    {
      Serial.print("Sd Error");
      digitalWrite(2,HIGH);
      delay(1000);
      digitalWrite(2,LOW);
      delay(5000);
    }
  }
  else if (!SerialFlash.begin(flashCS)) {
    isStopped = true;

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
    isStopped = true;

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
     
    }
    
  }

  //Creates/Opens files
  baromData = SD.open("BarometerData.txt", FILE_WRITE);
  flashBaromData = SerialFlash.open("baromData.txt");
  acelData = SD.open("AccelerometerData.txt", FILE_WRITE);
  flashAcelData = SerialFlash.open("acelData.txt");
  gyroData = SD.open("GyroscopeData.txt", FILE_WRITE);
  flashGyroData = SerialFlash.open("gyroData.txt");
}

void loop() {
  digitalWrite(4,HIGH);

  //If they're are any errors this will stop the code
  if (Serial.readString() == "stop")
  {
    isStopped = true;
  }

  //Writes data for barometer
  if (isStopped == false && flashBaromData && baromData) {
    float altitude = getAltitude(bmp.readPressure(), bmp.readTemperature());
    altStr.concat(altitude);
    altStr = altStr + ", ";

    baromData.println(altStr);
    flashBaromData.write(buf, altitude);
  
    delay(500);
    altStr = "";
  }

  //Writes data for accelerometer
  if (isStopped == false && flashAcelData && acelData){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Acel[0].concat(a.acceleration.x);
    Acel[1].concat(a.acceleration.y);
    Acel[2].concat(a.acceleration.z);
    
    Acel[0] = "X Acel:"+ Acel[0] + ", ";
    Acel[1] = "Y Acel:"+ Acel[1] + ", ";
    Acel[2] = "Z Acel:"+ Acel[2] + ", ";

    acelData.print(Acel[0]);
    acelData.print(Acel[1]); 
    acelData.print(Acel[2]); 
    flashAcelData.write(buf, a.acceleration.x);
    flashAcelData.write(buf, a.acceleration.y); 
    flashAcelData.write(buf, a.acceleration.z);
    
    Acel[0] = "";
    Acel[1] = "";
    Acel[2] = ""; 
    delay(500); 
  }

  //Writes Data for gyroscope
  if (isStopped == false && flashGyroData && gyroData)
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    gyroStr.concat(g.gyro.x);
    gyroStr = gyroStr + ", ";

    gyroData.print(gyroStr);
    flashGyroData.write(buf, g.gyro.x);

    gyroStr = "";
    delay(500);
  }
    
}

