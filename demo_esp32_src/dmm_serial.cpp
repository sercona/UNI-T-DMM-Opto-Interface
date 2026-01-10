/*
    dmm_serial.cpp

    (c) 2021-2025 linux-works labs
*/

#include <Arduino.h>

#include "dmm_serial.h"

#include "project_settings.h"



// the serial port
#ifdef ARDUINO_ARCH_ESP32
HardwareSerial          dmm_serial_port(1);      // hw serial for esp32
#else
EspSoftwareSerial::UART dmm_serial_port;         // sw serial for esp8266
#endif


// a local class
DmmSerial dmm_serial;        // call its loop() in our loop() to get serial data

// the meter decoder class
lw_ES519XX dmm;


// ctor
DmmSerial::DmmSerial (void)
{
  _new_data = false;
  _ndx = 0;
  bzero(_received_chars, MAX_USER_SERIAL_BUF);
}


//
// setup software serial on 2 'spare' gpio's if esp8266
// else use hardware uart on 2 spare pins
//

void DmmSerial::setup (void)
{
  // sw serial if esp8266
#ifndef ARDUINO_ARCH_ESP32
  dmm_serial_port.begin(19200,
                        EspSoftwareSerial::SWSERIAL_7O1,  // 7 bits, odd-parity, 1 stop-bit
                        D3,       // from opto, with 200ohm pullup to 3.3v
                        D0,       // not needed, dummy entry
                        false);   // not inverted mode (polarity)

#else
  // hw serial if esp32
  dmm_serial_port.begin(19200,
                        SERIAL_7O1,
                        PIN_RXD2,
                        PIN_TXD2);
#endif

  delay(100);

  memset(last_val_buf, 0, 255);
}


//
// process whole line (14 bytes?) from DMM meter
//  write values that we get from serial into program global vars.
//

void DmmSerial::_process_dmm_serial_data (void)
{
  //
  // parse the serial buffer and return the 'dmm.val_as_str' and 'dmm.meaning' fields
  //

  int ret = dmm.parse(_received_chars);

  if (ret) {
    // save the timestamp of that meter sample
    sprintf(last_val_timestamp, "%lu", millis());

    // save to 'read-only' read (ie, the webserver will only read this; we are the only writer of it)
    memset(last_val_buf, 0, 255);
    strncpy(last_val_buf, dmm.val_as_str, 255);
    strcat(last_val_buf, " ");
    strcat(last_val_buf, dmm.meaning);

    // optionally, print that here to another serial port
    Serial.println(last_val_buf);


#ifdef NOTYET
    // create a JSON single line string that we can send to mqtt and/or serial
    strcpy(json_s, "{\"ts\":\"");
    strcat(json_s, last_val_timestamp);
    strcat(json_s, "\",\"Value\":\"");
    strcat(json_s, dmm.val_as_str);
    strcat(json_s, "\",\"Units\":\"");
    strcat(json_s, dmm.meaning);
    strcat(json_s, "\"}");
#endif
  }
}


void DmmSerial::_recv_with_end_marker (void)
{
  char rc;

  _new_data = false;  // each time we're called, we reset this

  while ( (dmm_serial_port.available() > 0) && (_new_data == false) ) {
    // make sure we don't overrun our buffer!
    if (_ndx >= MAX_USER_SERIAL_BUF - 1) {
      _ndx = 0;
      _new_data = true;
      _received_chars[MAX_USER_SERIAL_BUF - 1] = '\0'; // limit giants
    }

    rc = dmm_serial_port.read(); // get a single char from the 'uart'


    // have we found our end-of-line char?  if so, close this string and return it to the user
    if (rc == '\n') {
      _received_chars[_ndx++] = '\n'; // in our case, we DO want to include the newline char
      _received_chars[_ndx] = '\0';   // ... and THEN terminate the newline-term'd string

      if (_ndx < 3) {  // ignore runts
        _new_data = false;
        _ndx = 0;
      } else {  // valid newline-delim'd string!  return to user (_new_data is 'true') and they will use it
        _ndx = 0;
        _new_data = true;
        return;
      }

    } else {
      _received_chars[_ndx++] = rc;  // consume the new single char and go back for more
    }

  } // while serial device has unread data AND no newline (EOL) gotton yet
}


void DmmSerial::loop (void)
{
  char *rx_chars = NULL;

  if (dmm_serial_port.available()) {
    _recv_with_end_marker();  // get a full line
    if (!_new_data) {         // discard empty lines
      return;
    }

    // rx_chars points to the whole string buffer, after a newline was received
    rx_chars = (char *)_received_chars;

#ifdef VERBOSE_SERIAL_TTY1
    Serial.print("\nrx: [");
    for (int i = 0; i < strlen(rx_chars); i++) {
      Serial.print(rx_chars[i], HEX); Serial.print(" ");
    }
    Serial.print("]\n");
    Serial.flush();
#endif

    // parse the DMM binary struct
    _process_dmm_serial_data();

  } // serial available
}


// end dmm_serial.cpp
