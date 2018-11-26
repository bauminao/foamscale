//
// Workflow:                                      Schematics: 
// ---------                                      -----------
// * Pin 3, 4 are GND.                                                   LED
// * Pin 3 is changed to ADC                               SCL --> D1   /
// * While measurement LED is LOW.                           \        /
// * Pin 4 goes HIGH.                                   VCC   \       |     SCA ---> D2
// * measure time V(t) on Pin 3                           |    \      |     |
// * i2c is connected to 2 (SCL) and 0 (SCA)           ---o-----o-----o-----o---
// * Time / Capacity is sent to i2c master.           |         2     1     0   |
// * Measurement is finished, LED goes HIGH.           \                        |
//                                                      |     ATTINY 85         |
//                                                     /                        |
//                                                    |         3     4         |
//                                                     ---o-----o-----o-----o---
//                                                        |     |     |     |
//                                                        |     |     R    GND
//                                                       RST    |     R
//                                                              |     R R=7.3Meg
//                                                              |     |
//                                                              +-----|
//                                                              |
//                                                          C =====
//                                                              |
//                                                             GND
// 
// 
// 
//
// About Fuses:
// http://www.engbedded.com/fusecalc/
//
