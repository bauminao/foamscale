#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

#define RST_OLED 16

#define datasize 40 
#define measurepoints 10

#define SLAVE_I2C 0x04

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.println("Setting OLED");

  pinMode(RST_OLED, OUTPUT);
  digitalWrite(RST_OLED, LOW); // turn D2 low to reset OLED
  delay(50);
  digitalWrite(RST_OLED, HIGH); // while OLED is running, must set D2 in high`

  Serial.println("Begin I2C");
  Wire.begin(D1,D2);

  //u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.drawString(0,0,"Time To Flash");

  Serial.print("Some time to flash - ");
  Serial.print("4 ");
  delay(1000);
  Serial.print("3 ");
  delay(1000);
  Serial.print("2 ");
  delay(1000);
  Serial.print("1 ");
  delay(1000);
  Serial.println("0 ");

  Serial.println("Starting Application");
  Serial.println("--------------------");
  Serial.println("");
}

void loop() {
  char buf[8];
  byte receive[datasize] = {};
  int  time    [measurepoints] = {};
  int  voltage [measurepoints] = {};

  Serial.println("Receiving data via i2s");
  Serial.println("Counter : ");
  //u8x8.drawString(0,0,"Reading       ");

  byte i = 0;

  for (byte reader = 0 ; reader < datasize ; reader ++)
  {
    Wire.requestFrom(4, 1); 
    Serial.print ("asking: ");
    Serial.print (i);

    while(Wire.available()) 
    {
      receive[i] = Wire.read();
      Serial.print (" got ");
      Serial.print (i);
      Serial.print (" (");
      Serial.print (receive[i]);
      Serial.println (") ");
      i++;
    }
  }
  Serial.println(" - Done.");

  Serial.println("Converting bytes to data.");
  Serial.println(" # ; time ; voltage ");
  for (byte k=0 ; k < measurepoints ; k++)
  {
    time    [k] = ((receive[ k*4 + 0 ] & 0xFF) << 8) | (receive[ k*4 + 1 ] & 0xFF);
    voltage [k] = ((receive[ k*4 + 2 ] & 0xFF) << 8) | (receive[ k*4 + 3 ] & 0xFF);
    Serial.print (k);
    Serial.print (" ; "); 
    Serial.print (time[k]);
    Serial.print (" ; "); 
    Serial.println (voltage[k]);
  }
  Serial.println(" - Done.");

  Serial.println("--------------------");
  Serial.println("");
  Serial.println("");

  delay (5000);
}

