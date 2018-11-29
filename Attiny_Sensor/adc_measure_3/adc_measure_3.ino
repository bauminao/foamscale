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
#define CHARGE  4


#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address (remember to change this when adapting this example)
// Get this from https://github.com/rambo/TinyWire

#include <TinyWireS.h>

long StartTime = 1;
long EndTime   = 2;

int data1=29;
int data2=0;

uint8_t adc_lobyte = 0; // to hold the low byte of the ADC register (ADCL)
uint16_t voltage   = 0;

void requestEvent()
{
  byte high = (byte)(data1>>8);
  byte low  = data1 & 0xff;
  TinyWireS.send(high);
  TinyWireS.send(low);
  high = (byte)(data2>>8);
  low  = data2 & 0xff;
  TinyWireS.send(high);
  TinyWireS.send(low);
}

void initADC_8Bit()
{
  ADMUX =
     (1 << ADLAR) |     // left shift because I only use 8 bit.
            (0 << REFS1) |     // Reference Voltage
            (0 << REFS0) |     // Reference Voltage
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // 
            (1 << ADPS1) |     // 
            (0 << ADPS0);      // 
}

int measure()
{
  //++data2;
  ADCSRA |= (1 << ADSC);         // start ADC measurement
  while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
  return ADCH;

  // for 10-bit resolution:
  //adc_lobyte = ADCL; // get the sample value from ADCL
  //voltage = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte
}


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

  int i=0;
  int value[20];

  StartTime = micros();
  digitalWrite(LED,HIGH);
  digitalWrite(CHARGE,HIGH);
  //while(measure() < 85);
  for (i=0; i< 20; i++)
  {
    //value[i] = measure();
    value[i] = analogRead(ADC);
    
  }
  EndTime = micros();

  //data1 = (int) (EndTime - StartTime);
  data1 = value[2];
  data2 = value[3];

  digitalWrite(CHARGE, HIGH);            
  digitalWrite(LED,LOW);

  //data2 = (int)NumToShow;

  TinyWireS.begin(I2C_SLAVE_ADDRESS); 
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  TinyWireS_stop_check();
}


