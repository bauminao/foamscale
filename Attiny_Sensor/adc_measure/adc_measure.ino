/* Capacity Measurement via i2c                              
// ============================
// 
// ATtiny85 does Measurement.
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

int sensordata=29;

void initADC_10Bit()
// 10 Bit 
{
  ADMUX =
     (0 << ADLAR) |     // do not left shift result (for 10-bit values)
            (1 << REFS2) |     // Sets ref. voltage to internal 1.1V, bit 2
            (0 << REFS1) |     // Sets ref. voltage to internal 1.1V, bit 1
            (0 << REFS0) |     // Sets ref. voltage to internal 1.1V, bit 0
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // set prescaler to 128, bit 2
            (1 << ADPS1) |     // set prescaler to 128, bit 1
            (0 << ADPS0);      // set prescaler to 128, bit 0
}

void initADC_8Bit()
// 10 Bit 
{
  ADMUX =
     (0 << ADLAR) |     // do not left shift result (for 10-bit values)
            (1 << REFS2) |     // Sets ref. voltage to internal 1.1V, bit 2
            (0 << REFS1) |     // Sets ref. voltage to internal 1.1V, bit 1
            (0 << REFS0) |     // Sets ref. voltage to internal 1.1V, bit 0
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // set prescaler to 64, bit 2
            (1 << ADPS1) |     // set prescaler to 64, bit 1
            (1 << ADPS0);      // set prescaler to 64, bit 0
}

int measure_adc(void)
{
  initADC_10Bit();

  digitalWrite(PIN, LOW);

  uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)
  uint16_t raw_adc;

  ADCSRA |= (1 << ADSC);         // start ADC measurement
  while (ADCSRA & (1 << ADSC) ); // wait till conversion complete

  // for 10-bit resolution:
  adc_lobyte = ADCL; // get the sample value from ADCL
  raw_adc = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte

  return raw_adc;
}

int measure_cap(void)
{
  initADC_10Bit();
  uint16_t cap=0;
  uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)

  byte measure_count = 10;

  digitalWrite(PIN, LOW);
  digitalWrite(PIN, HIGH);

  uint16_t raw_adc[measure_count];
  for (byte i=0 ; i<measure_count ; i++)
  {
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete

    // for 10-bit resolution:
    adc_lobyte = ADCL; // get the sample value from ADCL
    raw_adc[i] = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte
  }

  cap = raw_adc[0];

  digitalWrite(PIN, LOW);

  return cap;
}



void requestEvent()
{
  byte high = (byte)(sensordata>>8);
  byte low  = sensordata & 0xff;
  TinyWireS.send(high);
  TinyWireS.send(low);
}


void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(ADC, INPUT);

  TinyWireS.begin(I2C_SLAVE_ADDRESS); 
  TinyWireS.onRequest(requestEvent);

  // Turn on LED when program starts
  digitalWrite(LED, HIGH);
  tws_delay(250);
  digitalWrite(LED,LOW);
  tws_delay(150);
  digitalWrite(LED,HIGH);
  tws_delay(250);
  digitalWrite(LED,LOW);
  tws_delay(150);
  digitalWrite(LED,HIGH);
}

void loop()
{
  //sensordata = 512;
  //sensordata = measure_cap();
  sensordata = measure_adc();
  TinyWireS_stop_check();

}


