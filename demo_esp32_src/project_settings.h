/*
   project_settings.h

   (c) 2023-2025 linux-works labs
*/

#ifndef _PROJ_SETTINGS_H_
#define _PROJ_SETTINGS_H_

// this is our main .ino filename (saved for showing build info via debug calls)
extern const char *MY_INO_FILE;

//
// define build-time options here
//

#define VERBOSE_SERIAL_TTY   // turn this OFF for production


//
// standard includes and special libs
//

#include <Arduino.h>

// DMM (chip)
#include <lw_es519xx.h>
extern lw_ES519XX  dmm;

// serial class (dmm to module and bt to esp)
#include "dmm_serial.h"
extern DmmSerial dmm_serial;      // call its loop() in our loop() to get serial data





#ifdef ARDUINO_ARCH_ESP32
extern HardwareSerial dmm_serial_port;

// serial uart
#define PIN_RXD2     2 // (18)
#define PIN_TXD2     3 // (19)

#else

#include <SoftwareSerial.h>
extern EspSoftwareSerial::UART dmm_serial_port;

#define ESP_OK      (0)
#endif


#define SERIAL_BUF_SIZ  256
#define LBUF_SIZE      1024
extern char     l_buf[LBUF_SIZE];         // general 'big string buffer'

extern char     last_val_buf[SERIAL_BUF_SIZ];        // save the ascii meter reading here
extern char     last_val_timestamp[32];
extern char     last_meter_value_s[SERIAL_BUF_SIZ];


// externs
uint8_t octet_to_bin(const char in_ch);
void string_mac_addr_to_six_octets(const char *mac_s, uint8_t *out_octets);


#endif // _PROJ_SETTINGS_H_

// end project_settings.h
