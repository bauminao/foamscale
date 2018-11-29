#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>


U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

#define RST_OLED 16

int deltaT = 0;

void setup() {
  pinMode(RST_OLED, OUTPUT);
  digitalWrite(RST_OLED, LOW); // turn D2 low to reset OLED
  delay(50);
  digitalWrite(RST_OLED, HIGH); // while OLED is running, must set D2 in high`

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Init");

  Wire.begin();

  Serial.begin(115200);

  delay(1000);
}

void loop()
{
  // Ask at sensor about measurement
  Wire.requestFrom(4, 4); // request 1 byte from slave device address 4

  int data = 0;
  char buf[5];
  byte receive[5];
  receive[0] = 0;
  receive[1] = 1;
  receive[2] = 2;
  receive[3] = 3;

  byte i = 0;

  while(Wire.available()) // slave may send less than requested
  {
    receive[i] = Wire.read();
    i++;
  }
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Reading");
  u8x8.drawString(0,2,"             ");
  u8x8.drawString(0,3,"             ");

  for (int j=0; j <= 3 ; j++)
  {
    Serial.println(receive[j]);
    itoa(receive[j], buf, 10);
    u8x8.drawString( (j*4) , 2 , buf);
  }

  int intvalue1 = ((receive[0] & 0xFF) << 8) | (receive[1] & 0xFF);
  int intvalue2 = ((receive[2] & 0xFF) << 8) | (receive[3] & 0xFF);

  deltaT = (intvalue2 - intvalue1);

  //u8x8.drawString( 0 , 3 , "Time:       x 8us");

  //itoa(deltaT, buf, 10);
  //u8x8.drawString( 6, 3 , buf);

  int intvalue = ((receive[0] & 0xFF) << 8) | (receive[1] & 0xFF);
  itoa(intvalue, buf, 10);
  u8x8.drawString( 0 , 3 , buf);

  intvalue = ((receive[2] & 0xFF) << 8) | (receive[3] & 0xFF);
  itoa(intvalue, buf, 10);
  u8x8.drawString( 8 , 3 , buf);


  delay(5000);
}

