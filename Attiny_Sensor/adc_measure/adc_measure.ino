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

int i=1;
bool an=true;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(ADC, INPUT);

  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  //TinyWireS.onReceive(receiveEvent); // not using this
  TinyWireS.onRequest(requestEvent);

  // Turn on LED when program starts
  digitalWrite(LED, HIGH);
  tws_delay(500);
  digitalWrite(LED,LOW);
  tws_delay(500);
  digitalWrite(LED,HIGH);
  tws_delay(500);
  digitalWrite(LED,LOW);
  tws_delay(500);
  digitalWrite(LED,HIGH);
}

void loop()
{
    // This needs to be here
    TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    byte voltage=measure();
    TinyWireS.send(voltage);
    //TinyWireS.send(i);
    if (an) 
    {
      digitalWrite(LED,LOW); 
      an=false;
    }
    else 
    { 
      digitalWrite(LED,HIGH);
      an=true;
    }

}

void initADC()
{
  /* this function initialises the ADC

        ADC Prescaler Notes:
 --------------------

    ADC Prescaler needs to be set so that the ADC input frequency is between 50 - 200kHz.

           For more information, see table 17.5 "ADC Prescaler Selections" in
           chapter 17.13.2 "ADCSRA – ADC Control and Status Register A"
          (pages 140 and 141 on the complete ATtiny25/45/85 datasheet, Rev. 2586M–AVR–07/10)

           Valid prescaler values for various clock speeds

      Clock   Available prescaler values
           ---------------------------------------
             1 MHz   8 (125kHz), 16 (62.5kHz)
             4 MHz   32 (125kHz), 64 (62.5kHz)
             8 MHz   64 (125kHz), 128 (62.5kHz)
            16 MHz   128 (125kHz)

           Below example set prescaler to 128 for mcu running at 8MHz
           (check the datasheet for the proper bit values to set the prescaler)
  */

  // 8-bit resolution
  // set ADLAR to 1 to enable the Left-shift result (only bits ADC9..ADC2 are available)
  // then, only reading ADCH is sufficient for 8-bit results (256 values)

  ADMUX =
            (1 << ADLAR) |     // left shift result
            (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
            (0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // set prescaler to 64, bit 2
            (1 << ADPS1) |     // set prescaler to 64, bit 1
            (0 << ADPS0);      // set prescaler to 64, bit 0
}

byte measure(void)
{
  initADC();
  ++i;

  ADCSRA |= (1 << ADSC);         // start ADC measurement
  /*
  while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
  /*

  if (ADCH > 128)
    {
     // ADC input voltage is more than half of VCC

    } else {

     // ADC input voltage is less than half of VCC

    } 

  //return ADCH;
  */
  return i;

}

