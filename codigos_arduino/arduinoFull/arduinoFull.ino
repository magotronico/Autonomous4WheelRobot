#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// MPU6050 Variables
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
unsigned long lastPrintTime = 0; // Variable to store the last print time
const long printInterval = 10; // Print interval in milliseconds

// GPS Variables
TinyGPSPlus gps;
SoftwareSerial gpsSerial(2, 3); // RX, TX
double currPos[2]; // Current position
double prevPos[2] = {0.000000000,0.000000000}; // Previous position

// Path planing variables
long double dist = 0;
float angle = 0;
unsigned int phase = 0; // counter % 2 = 0 -> straight, counter % 2 = 1 -> turn

// Motor Variables
const int in1 = 2; // Pin IN1 del L298N para el motor izquierdo
const int in2 = 3; // Pin IN2 del L298N para el motor izquierdo
const int in3 = 4; // Pin IN3 del L298N para el motor derecho
const int in4 = 5; // Pin IN4 del L298N para el motor derecho

const int enA = 6; // Pin ENA del L298N para controlar la velocidad del motor izquierdo
const int enB = 9; // Pin ENB del L298N para controlar la velocidad del motor derecho

const int velocidadMaxima = 240; // Ajusta la velocidad máxima según sea necesario

void setup() {
  // Serial Monitor Setup (for ESP82)
  Serial.begin(9600);

  // Motor Setup
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // IMU Setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  calibrateIMU(); // Calibrate IMU
  delay(20);

  // GPS Setup
  gpsSerial.begin(9600);
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {
        // Get current time
        unsigned long currentMillis = millis();
        // Compute distance and angle
        computeData();
        
        // Path Planning Algorithm
        if((phase % 2 == 0) && (dist <= 10) && (phase <= 8)){
          go();
        }
        if((phase % 2 != 0) && (angle <= 91) && (phase <= 8)){
          turnRight();
        }
      
        // Check if it's time to update Server with new data
        if (currentMillis - lastPrintTime >= printInterval) {
          lastPrintTime = currentMillis; // Update the last print time
          sendData();
        }

        prevPos = currPos; //Update previous position
      }
    }
  }
}

void computeData() {
  // Get current position
  currPos = {gps.location.lat(), gps.location.lng()};

  // Compute distance
  dist = sqrt(pow(currPos[0] - prevPos[0], 2) + pow(currPos[1] - prevPos[1], 2));

  // Compute angle:
  readIMUData(); // Read accelerometer and gyroscope data
  calculateAngles(); // Calculate roll, pitch, yaw
}

void sendData(){
  // Print the values on the serial monitor
  // Serial.print("Roll: "); Serial.print(accAngleX);
  // Serial.print(" Pitch: "); Serial.print(accAngleY);

  // Print Latitude, Longitude, and Angle
  Serial.print(gps.location.lat(), 9);
  Serial.print(",");
  Serial.print(gps.location.lng(), 9);
  Serial.print(",");
  Serial.println(yaw);
}

void readIMUData() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
}

void calculateAngles() {
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX;
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorY;

  gyroAngleX += (GyroX - GyroErrorX) * elapsedTime;
  gyroAngleY += (GyroY - GyroErrorY) * elapsedTime;
  yaw += (GyroZ - GyroErrorZ) * elapsedTime;
}

void calibrateIMU() {
  float sumAccX = 0, sumAccY = 0, sumGyroX = 0, sumGyroY = 0, sumGyroZ = 0;

  for (int i = 0; i < 200; i++) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

    sumAccX += atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI;
    sumAccY += atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI;

    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();

    sumGyroX += GyroX / 131.0;
    sumGyroY += GyroY / 131.0;
    sumGyroZ += GyroZ / 131.0;
  }

  AccErrorX = sumAccX / 200;
  AccErrorY = sumAccY / 200;
  GyroErrorX = sumGyroX / 200;
  GyroErrorY = sumGyroY / 200;
  GyroErrorZ = sumGyroZ / 200;
}

void go() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);
}

void stopMotors() {
  // Stop motors
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
analogWrite(enA, 0);
analogWrite(enB, 0);
}
