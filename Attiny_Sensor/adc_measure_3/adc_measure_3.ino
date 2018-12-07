/* Capacity Measurement via i2c                              
// ============================
// 
// ATtiny85 does Measurement.
// bauminao
*/

#define LED  1
#define SCL  2
#define SCA  0
#define ADC  4
#define CHARGE  3

#define datasize 32

#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address (remember to change this when adapting this example)
// Get this from https://github.com/rambo/TinyWire

#include <TinyWireS.h>

long StartTime = 0;
long EndTime   = 0;

byte dataptr = 0;
int data[datasize]={};

uint8_t adc_lobyte = 0; // to hold the low byte of the ADC register (ADCL)
uint16_t voltage   = 0;

void requestEvent()
{
  digitalWrite(LED,HIGH);
  byte high = 0;
  byte low  = 0;
  for (int i=0 ; i < datasize ; i++) 
  {
    high = (byte)(data[i]>>8);
    low  = data[i] & 0xff;
    TinyWireS.send(high);
    TinyWireS.send(low);
  }
  digitalWrite(LED,LOW);
}


//void initADC_8Bit()
//{
//  ADMUX =
//     (1 << ADLAR) |     // left shift because I only use 8 bit.
//            (0 << REFS1) |     // Reference Voltage
//            (0 << REFS0) |     // Reference Voltage
//            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
//            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
//            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
//            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0
//
//  ADCSRA =
//            (1 << ADEN)  |     // Enable ADC
//            (1 << ADPS2) |     // 
//            (1 << ADPS1) |     // 
//            (0 << ADPS0);      // 
//}

//int measure()
//{
//  //++data2;
//  ADCSRA |= (1 << ADSC);         // start ADC measurement
//  while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
//  return ADCH;
//
//  // for 10-bit resolution:
//  //adc_lobyte = ADCL; // get the sample value from ADCL
//  //voltage = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte
//}


void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(CHARGE, OUTPUT);

  // Turn on LED when program starts
  digitalWrite(LED, HIGH);
  tws_delay(250);
  digitalWrite(LED,LOW);
  tws_delay(250);
  digitalWrite(LED,HIGH);
  tws_delay(250);
  digitalWrite(LED,LOW);
  tws_delay(500);

  
  //initADC_8Bit();
  analogReference(DEFAULT);

  StartTime = micros();
  digitalWrite(LED,HIGH);
  digitalWrite(CHARGE,HIGH);
  //while(measure() < 85);
  int i = 0;
  for (i=0; i< datasize ; i = i + 2)
  {
    //value[i] = measure();
    data[i]   = micros();
    data[i+1] = analogRead(ADC);
  }
  EndTime = micros();

  //data1 = (int) (EndTime - StartTime);
  //data2 = StartTime;

  digitalWrite(CHARGE, LOW);
  digitalWrite(LED,LOW);

  TinyWireS.begin(I2C_SLAVE_ADDRESS); 
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  TinyWireS_stop_check();
}


