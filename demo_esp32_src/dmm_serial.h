/*
    dmm_serial.h

    (c) 2021-2026 linux-works labs
*/

#ifndef _DMM_SERIAL_H_
#define _DMM_SERIAL_H_

#include <Arduino.h>

#include <HardwareSerial.h>      // might be optional

#define MAX_USER_SERIAL_BUF  64


class DmmSerial {
  public:
    DmmSerial(void);    // ctor
    void setup(void);    // user calls this
    void loop(void);    // public api to run our process/dispatch loop

  private:
    void     _process_dmm_serial_data(void);
    void     _recv_with_end_marker(void);
    uint8_t  _received_chars[MAX_USER_SERIAL_BUF];  // an array to store the received data
    boolean  _new_data;
    uint8_t  _ndx;
};



#endif // _DMM_SERIAL_H_
