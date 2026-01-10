/*
   ut61e_serial.ino

  esp8266 serial proxy for uni-t ut61e DMM meter

  (c) 2023-2026 linux-works
*/

const char *MY_INO_FILE = __FILE__;  // capture the .ino filename and save it

#include <Arduino.h>

#include "project_settings.h"


char l_buf[LBUF_SIZE];         // general 'big string buffer'
char last_val_buf[SERIAL_BUF_SIZ];        // save the ascii meter reading here
char last_val_timestamp[32];
char last_meter_value_s[SERIAL_BUF_SIZ];


//
// single digit hex ascii chars to binary
//

uint8_t octet_to_bin (const char in_ch)
{
  if (in_ch >= 0 && in_ch <= '9') {
    return in_ch - '0';
  } else {
    return tolower(in_ch) - 'a' + 10;
  }
}


//
// convert strings of the format:
//  "24:D7:EB:CB:48:85"
//   01234567890123456    (index into string)
// to a sequence of 6 octets in pure 8bit binary
//  {0x24, 0xD7, 0xEB, 0xCB, 0x48, 0x85}
//
// unit test: string_mac_addr_to_six_octets((const char*)"24:D7:EB:CB:48:85", espnow_peer_mac_addr);

void string_mac_addr_to_six_octets (const char *mac_s, uint8_t *out_octets)
{
  uint8_t i = 0, j = 0;

  // "24"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
  j += 2; // ":"

  // "D7"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
  j += 2; // ":"

  // "EB"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
  j += 2; // ":"

  // "CB"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
  j += 2; // ":"

  // "48"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
  j += 2; // ":"

  // "85"
  out_octets[i++] = (octet_to_bin(mac_s[j]) << 4) | octet_to_bin(mac_s[j]);
}



// main setup
void setup (void)
{
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
  // are placed in * storage
  //

  dmm_serial.loop();    // give the serial class some runtime
}


// end ut61e_serial.ino
