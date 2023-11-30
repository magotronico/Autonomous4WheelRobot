#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

char ssid[] = "Galaxy S20";   // your network SSID (name) 
char pass[] = "IswallowIt";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 2331704;
const char * myWriteAPIKey = "KE2VPTXI69Q3QOLE";

unsigned long previousMillis = 0;        // will store last time ThingSpeak was updated
unsigned long previousWiFiMillis = 0;    // will store last time WiFi connection was checked
const long interval = 15000;             // interval at which to update ThingSpeak (15 seconds)
const long wifiInterval = 5000;          // interval to check WiFi connection

void setup() {
  Serial.begin(9600);  // Initialize serial
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  unsigned long currentMillis = millis();

  // Check WiFi connection
  if(WiFi.status() != WL_CONNECTED) {
    if(currentMillis - previousWiFiMillis >= wifiInterval) {
      previousWiFiMillis = currentMillis;
      WiFi.begin(ssid, pass);
    }
  }

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    float field1 = getValue(data, ',', 0).toFloat();
    float field2 = getValue(data, ',', 1).toFloat();
    float field3 = getValue(data, ',', 2).toFloat();
    
    // Set the fields with the values
    ThingSpeak.setField(1, field1);
    ThingSpeak.setField(2, field2);
    ThingSpeak.setField(3, field3);

    // Check if it's time to update ThingSpeak
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Write to the ThingSpeak channel
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if (x == 200) {
        Serial.println("Channel update successful.");
      } else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
    }
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
