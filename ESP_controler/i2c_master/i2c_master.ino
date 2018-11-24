#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>


U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C


void setup() {
  u8x8.begin();
  u8x8.setPowerSave(false);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Init");

  Wire.begin();

  Serial.begin(115200);


  delay(1000);
}

void loop()
{
  u8x8.begin();
  u8x8.setPowerSave(false);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Reading");

  Wire.requestFrom(4, 1); // request 1 byte from slave device address 4
 
  while(Wire.available()) // slave may send less than requested
    {
    int i = Wire.read(); // receive a byte as character
    Serial.println(i); // print the character
    char buf[12];
    sprintf (buf, "%1i", i);
    u8x8.drawString(0,2,"       ");
    u8x8.drawString(0,2,buf);
    u8x8.refreshDisplay();  // only required for SSD1606/7
    }
   
  delay(1000);
}

