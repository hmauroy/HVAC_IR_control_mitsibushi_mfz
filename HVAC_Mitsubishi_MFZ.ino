/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * Version update Nov, 2014
 * Mitsubishi HVAC protocol added by Vincent Cruvellier.
 *  delaymicrosecond() modified to avoid limit.
 * Version update Dec, 2014
 * Panasonic HVAC protocol added by Mathieu Thomas. 
 * Version update Dec, 2015
 * Mitsubishi HVAC protocol enhancement by Vincent Cruvellier.
 *   added    void sendHvacMitsubishiFD(
      HvacMode                  HVAC_Mode,           // Example HVAC_HOT  HvacMitsubishiMode
      int                       HVAC_Temp,           // Example 21  (°c)
      HvacFanMode               HVAC_FanMode,        // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
      HvacVanneMode             HVAC_VanneMode,      // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
      HvacAreaMode              HVAC_AreaMode,       // Example AREA_AUTO
      HvacWideVanneMode         HVAC_WideMode,       // Example WIDE_MIDDLE
      int                       HVAC_PLASMA,          // Example true to Turn ON PLASMA Function
      int                       HVAC_CLEAN_MODE,      // Example false 
      int                       HVAC_ISEE,            // Example False
      int                       OFF                   // Example false to Turn ON HVAC / true to request to turn off
    );
 * sendHvacMitsubishiFD Not tested, i don't have this material -- if someone can confirm then please push a note.
 * 
 * Version update, Feb 2019
 * Henrik Mauroy
 * hmauroy@gmail.com
 * hmauroy.github.io
 * 
 * 1) Included Serial handling by Nick Gammaond:
 *  http://www.gammon.com.au/serial
 * 2) Added control of lower vane on floor models like Mitsubishi MFZ-series. Byte 16 controls this.
 * 3) Input over serial interface: Data format 'A'[mode][temp][fanSpeed][upperVane][lowerVane][inOff] 
 *    e.g. A1232001 (A,Heat,23deg,speed 2, upperVane auto, lowerVane closed, ON)
 * 4) Changed how settings are handled to make it easier for inexperienced arduino coders to 
 *    use arrays containing only numbers for doing the varoious controls.
 * 
 */

#include "IRremote2_mfz.h"

IRsend irsend;

byte serialCount = 0;
String str = "";
byte settingsArray[6] = {1,23,2,0,0,1};  // [MODE, temp, speed, vanne_position, vane_lower, ON/OFF]
boolean onOff = false;
byte serialInArray[5];

// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 50;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  //IR Demo send a cmd To Mitsubishi HVAC
  Serial.println("IR remote Mitsubishi HVAC A1234001");
  Serial.println("Format: A1234001");
}


void loop() {

  // if serial data available, process it
  while (Serial.available () > 0) {
    processIncomingByte (Serial.read ());
  }
 
} // ends void loop


//////////////////////////// FUNCTIONS ///////////////////////////////////


// here to process incoming serial data after a terminator received
void process_data (char * data) // Data format A[mode][temp][fanSpeed][upperVane][lowerVane][inOff] 
                                // e.g.A1232001
  {
  // Check to see if we have valid data
  if(data[0] != 'A') {
    // Do nothing
    Serial.println("Faulty commands!"); 
  }
  else {
    //Serial.print("Sending data: ");
    //Serial.println(data);
    // Convert to String
    str = String(data[1]);
    settingsArray[0] = str.substring(0,1).toInt();
    str = String(data[2]) + String(data[3]);
    settingsArray[1] = str.substring(0,2).toInt();
    str = String(data[4]);
    settingsArray[2] = str.substring(0,1).toInt();
    str = String(data[5]);
    settingsArray[3] = str.substring(0,1).toInt();
    str = String(data[6]);
    settingsArray[4] = str.substring(0,1).toInt();
    str = String(data[7]);
    settingsArray[5] = str.substring(0,1).toInt();
   
    // Take care of the format of the on/off variable
    if(settingsArray[5] == 0) {
      onOff = true; // true turns OFF
    }
    else if(settingsArray[5] == 1) {
      onOff = false;
    }
    else {
      onOff = false;  // turning OFF is not what we want if a glitch happens
    }
    if(settingsArray[0] < 0 || settingsArray[0] > 4) {settingsArray[0] = 1;}  // Check if mode is out of bounds
    if(settingsArray[1] < 16) {settingsArray[1] = 16;}  // check temperature limits
    if(settingsArray[1] > 30) {settingsArray[1] = 30;}
    if(settingsArray[2] > 4) {settingsArray[2] = 1;}  // check fan speed limits
    if(settingsArray[2] < 1) {settingsArray[2] = 1;}
    if(settingsArray[3] > 6) {settingsArray[3] = 6;} // check vane position limits
    if(settingsArray[3] < 0) {settingsArray[3] = 0;}
    if(settingsArray[4] > 1) {settingsArray[4] = 1;} // check lower vane position limits
    if(settingsArray[4] < 0) {settingsArray[4] = 0;}
   //irsend.sendHvacMitsubishi_MFZ(HVAC_HOT, 23, FAN_SPEED_2, VANNE_AUTO, false);
   // Sending all commands as integers to the complicated codes in IRremote2_henrik.cpp.
   digitalWrite(13, HIGH);
   irsend.sendHvacMitsubishi_MFZ(settingsArray[0],settingsArray[1],settingsArray[2],settingsArray[3],settingsArray[4], onOff);
   delay(100);
   digitalWrite(13, LOW);
  }
 

}  // end of process_data

/////////////////////////////////////////////////////////////////////////////



void clearSerialInArray() {
  for(int i=0; i<serialCount; i++) {
    serialInArray[i] = 0;
    serialCount = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////


void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ... (Jumps over to process_data())
      process_data (input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch
   
  } // end of processIncomingByte  


/////////////////////////////////////////////////////////////////////////////












