#include <SoftwareSerial.h>
SoftwareSerial gsm(7, 8); // RX, TX pins

const int soilSensor = A0;
const int pumpRelay = 13;
int threshold = 500; // Adjust based on sensor calibration

void setup() {
  pinMode(pumpRelay, OUTPUT);
  digitalWrite(pumpRelay, LOW); // Keep pump off initially
  Serial.begin(9600);
  gsm.begin(9600);
  delay(10000); // Wait for network connection
  gsm.println("AT+CMGF=1"); // Set SMS mode
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0"); // Configure SMS notification
  delay(1000);
}

void loop() {
  int moisture = analogRead(soilSensor);
  
  // Automatic Control
  if (moisture > threshold) {
    digitalWrite(pumpRelay, HIGH); // Turn on pump
  } else {
    digitalWrite(pumpRelay, LOW); // Turn off pump
  }

  // SMS Control/Status Check
  if (gsm.available() > 0) {
    String msg = gsm.readString();
    if (msg.indexOf("STATUS") > 0) {
      sendSMS("Moisture: " + String(moisture) + " Pump: " + (digitalRead(pumpRelay) ? "ON" : "OFF"));
    }
  }
}

void sendSMS(String message) {
  gsm.println("AT+CMGS=\"+91XXXXXXXXXX\""); // <-- YOUR NUMBER
  delay(1000);
  gsm.print(message);
  delay(100);
  gsm.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}
