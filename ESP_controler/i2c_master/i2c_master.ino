#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>


U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

#define RST_OLED 16

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
  Wire.requestFrom(4, 2); // request 1 byte from slave device address 4

  int data = 0;
  char buf[5];
  byte receive[4];
  receive[0] = 1;
  receive[1] = 1;
  receive[2] = 1;
  receive[3] = 1;

  byte i = 0;

  while(Wire.available()) // slave may send less than requested
  {
    receive[i] = Wire.read();
    i++;
  }
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Reading");
  u8x8.drawString(0,2,"       ");

  for (int j=0; j < 4; j++)
  {
    Serial.println(receive[j]);
    itoa(receive[j], buf, 10);
    u8x8.drawString( (j*4) , 2 , buf);
  }

  delay(5000);
}

