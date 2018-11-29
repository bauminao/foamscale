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

long StartTime = 0;
long EndTime   = 0;
float uF = 0.0;
float nF = 0.0;
float pF = 0.0;
int NumToShow = 0;
int CurNum = 0;
boolean HalfTimeConstant = LOW;
float Divider = 0;                                                                                                                  
const float Resistance = 40.0;


int data1=29;
int data2=29;

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


void setup()
{
  analogReference(DEFAULT);
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

  digitalWrite(LED,HIGH);
  digitalWrite(CHARGE,HIGH);

  StartTime = micros();
  data1     = (int) StartTime;
  while(analogRead(ADC) < 881){// 86%of supply voltage  
        while(analogRead(ADC) < 255){
          HalfTimeConstant = HIGH;
        }
        if((micros()-StartTime) >= 10000 && HalfTimeConstant == HIGH){// 500000uS is the time taken to charge a 1uF cap to 39% of supply voltage
          Divider = 0.25;
          break;        
        }
        else{
          Divider = 2.0;
          HalfTimeConstant = LOW;
        }
  }
   
  EndTime = (micros() - StartTime)/Divider;
  data2 = (int) micros();
  uF = ((float)EndTime/Resistance);//F=T/R
  if(uF >= 1){
    NumToShow = uF;
  }
  else {
      nF = uF*1000.0;
      if(nF >= 1){
        NumToShow = nF;
      }
      else{
        pF = nF*1000.0;
        NumToShow = pF;
      }
  }
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


