#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
unsigned long lastPrintTime = 0; // Variable to store the last print time
const long printInterval = 10; // Print interval in milliseconds

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  calibrateIMU(); // Calibrate IMU
  delay(20);
}

void loop() {
  unsigned long currentMillis = millis();

  // Read accelerometer and gyroscope data
  readIMUData();

  // Calculate roll, pitch, yaw
  calculateAngles();

  // Check if it's time to print values
  if (currentMillis - lastPrintTime >= printInterval) {
    lastPrintTime = currentMillis; // Update the last print time

    // Print the values on the serial monitor
    // Serial.print("Roll: "); Serial.print(accAngleX);
    // Serial.print(" Pitch: "); Serial.print(accAngleY);
    Serial.print(" Yaw: "); Serial.println(yaw);
  }
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
