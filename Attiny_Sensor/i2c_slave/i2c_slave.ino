/* Capacity Measurement via i2c                              
// ============================
// 
// ATtiny85 does Measurement.
//
// Workflow:                                      Schematics: 
// ---------                                      -----------
// * Pin 3, 4 are GND.                                                   LED
// * Pin 3 is changed to ADC                               SCL --> D1   /
// * While measurement LED is LOW.                           \        /
// * Pin 4 goes HIGH.                                   VCC   \       |     SCA ---> D2
// * measure time V(t) on Pin 3                           |    \      |     |
// * i2c is connected to 2 (SCL) and 0 (SCA)           ---o-----o-----o-----o---
// * Time / Capacity is sent to i2c master.           |         2     1     0   |
// * Measurement is finished, LED goes HIGH.           \                        |
//                                                      |     ATTINY 85         |
//                                                     /                        |
//                                                    |         3     4         |
//                                                     ---o-----o-----o-----o---
//                                                        |     |     |     |
//                                                        |     |     R    GND
//                                                       RST    |     R
//                                                              |     R R=7.3Meg
//                                                              |     |
//                                                              +-----|
//                                                              |
//                                                          C =====
//                                                              |
//                                                             GND
// 
// 
// 
//
// About Fuses:
// http://www.engbedded.com/fusecalc/
//
// bauminao
*/

#define LED  1
#define SCL  2
#define SCA  0
#define ADC  3
#define PIN  4

#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address (remember to change this when adapting this example)
// Get this from https://github.com/rambo/TinyWire

#include <TinyWireS.h>

int i=0;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(PIN, OUTPUT);

  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  //TinyWireS.onReceive(receiveEvent); // not using this
  TinyWireS.onRequest(requestEvent);

  // Turn on LED when program starts
  digitalWrite(LED, HIGH);
}

void loop()
{
    // This needs to be here
    TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    TinyWireS.send(i);
    digitalWrite(LED,LOW);
    tws_delay(500);
    digitalWrite(LED,HIGH);
    tws_delay(500);
    digitalWrite(LED,LOW);
    tws_delay(500);
    digitalWrite(LED,HIGH);

    i++;
}
