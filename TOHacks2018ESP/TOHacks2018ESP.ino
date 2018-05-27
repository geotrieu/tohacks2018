// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

//Shock
int bAlarmOutput = 11;
bool bAlarm = false;
unsigned long timeSinceAlarm = 0;
float gyroTol = 0;
float noABS = 0;

//SOIL
int sensor_pin = A0; 
int ledPin = 12;
int output_value ;

//ULTRA
#define trigPinU 6
#define echoPinU 5
#define trigPinL 4
#define echoPinL 3
float durationU, distanceU;
float durationL, distanceL;
float predur[10];
int vibPin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(bAlarmOutput, OUTPUT);
  //Serializing upper pins
  pinMode(trigPinU, OUTPUT);
  pinMode(echoPinU, INPUT);
  
  //Serializing lower pins
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(vibPin, OUTPUT);
  Serial.println("Reading From the Sensor ...");
  gyroSetup();
  }

void loop() {
  checkWater();
  //checkSonic();
  noABS = gyroLoop();
  gyroTol = abs(noABS);
  if (gyroTol < 10){
    if (bAlarm == false)
    timeSinceAlarm = millis();
    bAlarm = true;
    //digitalWrite(bAlarmOutput, HIGH);
  } else if (gyroTol > 10){
    bAlarm = false;
    timeSinceAlarm = 0;
    //digitalWrite(bAlarmOutput, LOW);
  }
    //Alarm Oscillating Beep
  if (bAlarm && timeSinceAlarm != 0) {
    Serial.println((millis() - timeSinceAlarm) % 1000 < 500);
    Serial.println(timeSinceAlarm);
    Serial.println(millis());
    if ((millis() - timeSinceAlarm) % 1000 < 500) {
      digitalWrite(bAlarmOutput, HIGH);
    } else {
      digitalWrite(bAlarmOutput, LOW);
    }
    
  } else {
    digitalWrite(bAlarmOutput, LOW);
  }
}
