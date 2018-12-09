/* Master control
   ==============

   Workflow:
    * First setup your own environment.
      Pre-Set the whole PINs and OLED.
      Give the rest of the world 5s to prepare
    * Loop will reset the attached sensors
                aks each one about measurement points
                calculate however a useable value
                push th value to the display manager.
                refresh the display manager
    
    bauminao
*/

#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

const byte RST_OLED      = 16;  // D0


//     ||                         ||                     ||
//     ||                         ||                     ||
//    \||/                       \||/                   \||/
//     \/                         \/                     \/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
const byte SENSOR_AMOUNT = 6;   

const byte RST_S01       = 0;   // D3
const byte RST_S02       = 15;  // D8
const byte RST_S03       = 13;  // D7
const byte RST_S04       = 12;  // D6 
const byte RST_S05       = 14;  // D5
const byte RST_S06       = 2;   // D4

const byte MEASUREPOINTS = 20;
const int  DATASIZE      = MEASUREPOINTS*4;

// Array with sensor IDs, but we start at #1, not #0
uint8_t SensorID [SENSOR_AMOUNT]  = { 11 , 12 , 13 , 14 , 15 , 16 };

const byte debug_level   = 1;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//     /\                         /\                     /\                  /
//    /||\                       /||\                   /||\
//     ||                         ||                     ||
//     ||                         ||                     ||


// The big global data-variable to store all the raw data.
uint8_t data[SENSOR_AMOUNT][DATASIZE]           = {};

// And the accoring value-array in int
int  meastime[SENSOR_AMOUNT][MEASUREPOINTS] = {};
int  voltage [SENSOR_AMOUNT][MEASUREPOINTS] = {};


// All resulting sensor data
int sensor[SENSOR_AMOUNT] = {};

// The sum of all:
int result = 0;

void TimeToFlash(byte count, int _delay) {
  Serial.print("Some time to flash - ");
  for (byte _counter=count ; _counter > 0 ; _counter--) {
    Serial.print(_counter);
    Serial.print(" ");
    delay(_delay);
  }
  Serial.println(" 0");
}

void DoSensorReset(int sensorid) {
  digitalWrite(sensorid , LOW);
  delay(100);
  digitalWrite(sensorid , HIGH);
}

void Sensor_Reset(int _delay) {
  if (debug_level > 0 ) { Serial.print("Resetting Sensor"); }
  if (debug_level > 2 ) { Serial.print(", S00"); }
  DoSensorReset(RST_S01);
  delay(_delay);
  if (debug_level > 2 ) { Serial.print(", S01"); }
  DoSensorReset(RST_S02);
  delay(_delay);
  if (debug_level > 2 ) { Serial.print(", S02"); }
  DoSensorReset(RST_S03);
  delay(_delay);
  if (debug_level > 2 ) { Serial.print(", S03"); }
  DoSensorReset(RST_S04);
  delay(_delay);
  if (debug_level > 2 ) { Serial.print(", S04"); }
  DoSensorReset(RST_S05);
  delay(_delay);
  if (debug_level > 2 ) { Serial.print(", S05"); }
  DoSensorReset(RST_S06);
  delay(_delay);
  if (debug_level > 0 ) { Serial.println(""); }
}

void RestartLoop(byte count, int _delay) {
  if (debug_level > 0 ) { Serial.print("Full Restart in ");}
  for (byte _counter=count ; _counter > 0 ; _counter--) {
    if (debug_level > 1 ) { 
      Serial.print("_counter");
      Serial.print(" ");
    }
    delay(_delay);
  }
  if (debug_level > 0 ) { Serial.println("0. --> Loop ");}
}


void read_Sensor(uint8_t sensorid){
  byte i = 0;
  uint8_t error=0;
  uint8_t dataamount = 1;
  for (byte _run = 0 ; _run < DATASIZE ; _run ++)
  {
    error = Wire.requestFrom(SensorID[sensorid], dataamount); 
    if (debug_level > 2 ) {  
      Serial.print("asking ");
      Serial.print (i);
    }

    while(Wire.available()) 
    {
      data[sensorid][i] = Wire.read();
      if (debug_level > 2 ) { 
        Serial.print (" got ");
        Serial.print (i);
        Serial.print (" (");
        Serial.print (data[sensorid][i]);
        Serial.println (") ");
      }
      i++;
    }
  }
}

void convert_byte_2_int(byte _sensor) {
  if (debug_level > 0 ) { Serial.println("Converting bytes to data.");}
  for (int k=0 ; k < MEASUREPOINTS ; k++) {
    meastime[_sensor][k] = ((data[_sensor][ k*4 + 0 ] & 0xFF) << 8) | (data[_sensor][ k*4 + 1 ] & 0xFF);
    voltage [_sensor][k] = ((data[_sensor][ k*4 + 2 ] & 0xFF) << 8) | (data[_sensor][ k*4 + 3 ] & 0xFF);
  }
}

void dump_sensor(byte _sensor) {
  Serial.println(" Sensor ; # ; time ; voltage ");
  for (int k=0 ; k < MEASUREPOINTS ; k++)  {
    Serial.print (SensorID[_sensor]);
    Serial.print (" ; "); 
    Serial.print (k);
    Serial.print (" ; "); 
    Serial.print (meastime[_sensor][k]);
    Serial.print (" ; "); 
    Serial.println (voltage[_sensor][k]);
  }
}

void calculate_values (byte _sensor) {
  Serial.println("Debug");
  Serial.println(_sensor);
  Serial.println("END Debug");

  sensor[_sensor] = 0;


  for (int i=0; i < MEASUREPOINTS ; i++)
  {
    sensor[_sensor] = sensor[_sensor] + voltage[_sensor][i];
  }
  if (debug_level > 1 ) { 
    Serial.print("Sensor #");
    Serial.print(_sensor);
    Serial.print(" -- ");
    Serial.println(sensor[_sensor]);
  }

  result = 0;
  for (int i=0; i < SENSOR_AMOUNT ; i++) 
  {
    result = result + sensor[i];
  }

  result = result / (SENSOR_AMOUNT - 1);

  if (debug_level > 0 ) { 
    Serial.print("SUM.Value: ");
    Serial.println(result);
  }
}

void DisplayManager() {
  char buf[8]="       ";
  u8x8.drawString(0,0,"                ");
  u8x8.drawString(0,1,"                ");
  u8x8.drawString(0,2,"                ");
  u8x8.drawString(0,3,"                ");

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);

  // Sensor 1
  itoa(int(sensor[0]) , buf , 10);
  u8x8.drawString(0,0,"     ");
  u8x8.drawString(0,0,buf);

  // Sensor 2
  itoa(int(sensor[1]) , buf , 10);
  u8x8.drawString(0,1,"      ");
  u8x8.drawString(0,1,buf);

  // Sensor 3
  itoa(int(sensor[2]) , buf , 10);
  u8x8.drawString(0,2,"      ");
  u8x8.drawString(0,2,buf);

  // Sensor 4
  itoa(int(sensor[3]) , buf , 10);
  u8x8.drawString(6,0,"      ");
  u8x8.drawString(6,0,buf);

  // Sensor 5
  itoa(int(sensor[4]) , buf , 10);
  u8x8.drawString(6,1,"      ");
  u8x8.drawString(6,1,buf);

  // Sensor 6
  itoa(int(sensor[5]) , buf , 10);
  u8x8.drawString(6,2,"      ");
  u8x8.drawString(6,2,buf);

  // Summenwert
  itoa(int(result) , buf , 10);
  u8x8.drawString(11,3,"      ");
  u8x8.drawString(11,3,buf);

//  u8x8.drawString(0,0,"amstrad_cpc_extf");
//  u8x8.drawString(0,1,"1234567890123456");
//  u8x8.drawString(0,2,"1234567890123456");
//  u8x8.drawString(0,3,"1234567890123456");
}






void setup() {
  // Serial connection
  Serial.begin(115200);
  Serial.println("Starting");

  // Init Display
  if (debug_level > 0 ) { Serial.println("Setting OLED");}
  pinMode(RST_OLED, OUTPUT);
  digitalWrite(RST_OLED, LOW); // turn D2 low to reset OLED
  delay(50);
  digitalWrite(RST_OLED, HIGH); // while OLED is running, must set D2 in high`

  // RAW sensor connections (reset-pins)
  if (debug_level > 0 ) { Serial.println("Setting Reset-Pins");}
  pinMode(RST_S01, OUTPUT);
  pinMode(RST_S02, OUTPUT);
  pinMode(RST_S03, OUTPUT);
  pinMode(RST_S04, OUTPUT);
  pinMode(RST_S05, OUTPUT);
  pinMode(RST_S06, OUTPUT);

  digitalWrite(RST_S01, HIGH); 
  digitalWrite(RST_S02, HIGH); 
  digitalWrite(RST_S03, HIGH); 
  digitalWrite(RST_S04, HIGH); 
  digitalWrite(RST_S05, HIGH); 
  digitalWrite(RST_S06, HIGH); 

  // Open I2C
  if (debug_level > 0 ) { Serial.println("Begin I2C");}
  Wire.begin();

  // Start Display
  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0,0,"                ");
  u8x8.drawString(0,1,"                ");
  u8x8.drawString(0,2,"                ");
  u8x8.drawString(0,3,"                ");
  u8x8.drawString(0,0,"Time To Flash");

  // Give the world 5s to prepare
  TimeToFlash( 5 , 1000 );

  // Prepare for showtime
  if (debug_level > 0 ) { 
    Serial.println("Starting Application");
    Serial.println("--------------------");
    Serial.println("");
  }
}




void loop() {
  // Read Sensor data
  for (uint8_t sensor = 0 ; sensor < SENSOR_AMOUNT ; sensor++) {
    if (debug_level > 0 ) {  
      Serial.print ("Aktive Sensor: ");
      Serial.println (SensorID[sensor]);
      Serial.println("Receiving data via i2c");
    }

    read_Sensor(sensor);
    convert_byte_2_int(sensor);
    if (debug_level > 1 ) { dump_sensor(sensor); }


    // Now, after all Sensordata is read, we should calculate something more useful:
    calculate_values(sensor);

    // Updating Display Manager
    DisplayManager();
  }

  // Reset all Sensors with an delay of 0.5s
  Sensor_Reset(1000);

  // Wait for Restart
  RestartLoop(2,100);
}


//  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
//  u8x8.setFont(u8x8_font_5x7_f);
//  u8x8.setFont(u8x8_font_chroma48medium8_r);
