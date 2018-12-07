#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>


U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

#define RST_OLED 16

#define datasize 64 

void setup() {
  pinMode(RST_OLED, OUTPUT);
  digitalWrite(RST_OLED, LOW); // turn D2 low to reset OLED
  delay(50);
  digitalWrite(RST_OLED, HIGH); // while OLED is running, must set D2 in high`

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Init  3");

  Wire.begin();

  Serial.begin(115200);

  delay(1000);
  u8x8.drawString(0,0,"Init  2");
  delay(1000);
  u8x8.drawString(0,0,"Init  1");
  delay(1000);
  u8x8.drawString(0,0,"Init  0");
  delay(1000);
}

void loop()
{
  // Ask at sensor about measurement
  char buf[5];
  byte receive[256] = {};
  int  data[datasize]        = {};

  byte i = 0;

  Wire.requestFrom(4, datasize); // request 1 byte from slave device address 4

  while(Wire.available()) // slave may send less than requested
  {
    u8x8.drawString(0,0,"Reading       ");
    itoa(i , buf , 10);
    u8x8.drawString( 8 , 0 , buf);
    receive[i] = Wire.read();
    i++;
  }
  for (int k=0; k < datasize ; k++)
  {
    data[k] = ((receive[k*2] & 0xFF) << 8) | (receive[k*2+1] & 0xFF);
  }
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.drawString(0,0,"Values       ");
  u8x8.drawString(0,1,"             ");
  u8x8.drawString(0,2,"             ");
  u8x8.drawString(0,3,"             ");

  for (byte j = 0; j < datasize ; j=j+2)
  {
    itoa(j , buf , 10);
    u8x8.drawString( 0 , 2 , buf);

    itoa(data[j] , buf , 10);
    u8x8.drawString( 0 , 3 , buf);

    itoa(j+1 , buf , 10);
    u8x8.drawString( 8 , 2 , buf);

    itoa(data[j+1] , buf , 10);
    u8x8.drawString( 8 , 3 , buf);

    delay (200);
  }


  u8x8.drawString(0,0,"Time to flash");
  delay(5000);
}

