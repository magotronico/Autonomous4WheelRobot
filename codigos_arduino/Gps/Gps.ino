#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial gpsSerial(2, 3); // RX, TX
TinyGPSPlus gps;

// Initialize mock latitude, longitude, and velocity
float latitude = 0.0;
float longitude = 0.0;
float mockVelocity = 0.0; // Initialize mock velocity

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  if (gpsSerial.available()) { // Check if GPS data is available
    if (gps.encode(gpsSerial.read())) {
      updateGPSValues(); // Read and update GPS data
      updateMockValues(); // Update the mock velocity
      printValues(); // Print latitude, longitude, and mock velocity
    }
  }
}

void updateGPSValues(){
  // Update latitude and longitude if GPS data is valid
  latitude = gps.location.isValid() ? gps.location.lat() : 0.0;
  longitude = gps.location.isValid() ? gps.location.lng() : 0.0;
}

void updateMockValues() {
  mockVelocity += 0.125; // Increment the mock velocity
}

void printValues(){
  Serial.print(latitude, 9);
  Serial.print(","); Serial.print(longitude, 9);
  Serial.print(","); Serial.println(mockVelocity, 3);
}
