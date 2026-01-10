/*
   ut61e_serial.ino

  esp8266/esp32 serial proxy for uni-t ut61e DMM meter

  (c) 2023-2026 linux-works
*/

const char *MY_INO_FILE = __FILE__;  // capture the .ino filename and save it

#include <Arduino.h>

#include "project_settings.h"


char l_buf[LBUF_SIZE];         // general 'big string buffer'
char last_val_buf[SERIAL_BUF_SIZ];        // save the ascii meter reading here
char last_val_timestamp[32];
char last_meter_value_s[SERIAL_BUF_SIZ];




// main setup
void setup (void)
{
  delay(1000);
  
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("\n"); Serial.flush();

  // DMM serial setup
  dmm_serial.setup();
  strcpy(last_meter_value_s, "0.0");  // put something in there so that next compare will fail
}



// the main arduino loop
void loop (void)
{
  //
  // run the dmm serial loop frequently to look for new chars and when the full packet is parsed, values
  // are placed in global storage
  //

  dmm_serial.loop();    // give the serial class some runtime
}


// end ut61e_serial.ino
