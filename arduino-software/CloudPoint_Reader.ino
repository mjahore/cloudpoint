// Adafruit library for the amplifier circuit.
#include <Adafruit_MAX31865.h>

// Use software SPI: CS, DI, DO, CLK
// These are the digital pins for the RTD temperature probe.
Adafruit_MAX31865 max = Adafruit_MAX31865(10, 11, 12, 13);

// inPin0 is the pin that is reading the voltage from the
// photodiode amplifier circuit.
#define inPin0 0

// RREF is the reference resistor on the breakout amplifier board.
#define RREF 430.0

void setup() {
  // Open the port to the Arduino.
  Serial.begin(9600);

  // Begin reading temperature
  max.begin(MAX31865_3WIRE);  
}

void loop() {

  // Read voltage from the analog pin.
  int pinRead0 = analogRead(inPin0);
  float pVolt0 = pinRead0 / 1024.0 * 5.0;
  
  // Variable for temperature.
  float tempRTD = max.temperature(100, RREF);
  
  // Write the voltage read to the serial out.

  //Serial.print("Temperature = "); Serial.println(max.temperature(100, RREF));
  Serial.print(tempRTD);
  Serial.print("\t");
  Serial.print(pVolt0);
  Serial.print("\n");
  
  // Check and print any faults from RTD probe.
  uint8_t fault = max.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    max.clearFault();
  }

  // Pause before taking another measurement.
  delay(1000);
}
