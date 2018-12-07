/* Capacity Measurement via i2c                              
// ============================
// 
// ATtiny85 does Measurement.
// bauminao
*/

#define LED 1
#define SCL 2
#define SCA 0 
#define ADC 4
#define CHARGE 3

#define runs     10              // Measurement points
#define datasize 40              // (2 byte for time + 2 byte for voltage ) x runs

#define I2C_SLAVE_ADDRESS 0x04

#include <TinyWireS.h>

byte         dataptr = 0;
byte data [datasize] = {};

uint8_t   adc_lobyte = 0;
uint16_t     voltage = 0;

void requestEvent() {
//  digitalWrite (LED, HIGH);
  TinyWireS.send(data[dataptr]);
  dataptr++;
  if (dataptr >= datasize) {
    dataptr = 0;
    digitalWrite(LED,LOW);
  }
  digitalWrite(LED,LOW);
}

void setup () {
  pinMode(LED    , OUTPUT);
  pinMode(CHARGE , OUTPUT);

  digitalWrite(LED , HIGH);
  tws_delay(350);
  digitalWrite(LED , LOW);
  tws_delay(200);
  digitalWrite(LED , HIGH);
  tws_delay(250);
  digitalWrite(LED , LOW);
  tws_delay(500);

  analogReference(DEFAULT);

  digitalWrite(   LED , HIGH);
  digitalWrite(CHARGE , HIGH);

  int run     = 0;
  int voltage [runs] = {};
  int    time [runs] = {};

  for (run = 0 ; run < runs ; run++)
  {
    time [run ]   = micros();
    voltage [run] = analogRead(2);
  }

  digitalWrite(CHARGE , LOW);
  digitalWrite(LED    , HIGH);

  for (run = 0 ; run < runs ; run++)
  {
    data [run * 4 + 0] = (byte) (time [run] >> 8  );  
    data [run * 4 + 1] = (byte) (time [run] & 0xff);
    data [run * 4 + 2] = (byte) (voltage [run] >> 8  );  
    data [run * 4 + 3] = (byte) (voltage [run] & 0xff);
  }

  digitalWrite(   LED , LOW);

  tws_delay(500);

  TinyWireS.begin(I2C_SLAVE_ADDRESS); 
  TinyWireS.onRequest(requestEvent);
}

void loop() {
  TinyWireS_stop_check();
}

