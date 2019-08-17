#ifndef SDS011_H
#define SDS011_H

#include <Arduino.h>

#define REPORT_ACTIVE_MODE  false
#define REPORT_QUERY_MODE   true

#define RESPONSE_SIZE 10
#define COMMAND_SIZE  19

#define ID_RESPONSE   0xC5
#define ID_VALUE      0xC0
#define ID_COMMAND    0xB4

#define HOST_PREAMBLE     0
#define HOST_COMMAND      1
#define HOST_DATA1        2
#define HOST_DATA2        3
#define HOST_DATA3        4
#define HOST_DATA4        5
#define HOST_DATA5        6
#define HOST_DATA6        7
#define HOST_DATA7        8
#define HOST_DATA8        9
#define HOST_DATA9        10
#define HOST_DATA10       11
#define HOST_DATA11       12
#define HOST_DATA12       13
#define HOST_DATA13       14
#define HOST_DATA14       15
#define HOST_DATA15       16
#define HOST_CHECKSUM     17
#define HOST_TAIL         18

#define SENSOR_PREAMBLE   0
#define SENSOR_COMMAND    1
#define SENSOR_DATA1      2
#define SENSOR_DATA2      3
#define SENSOR_DATA3      4
#define SENSOR_DATA4      5
#define SENSOR_DATA5      6
#define SENSOR_DATA6      7
#define SENSOR_CHECKSUM   8
#define SENSOR_TAIL       9

class SDS011Class {
  public:
    SDS011Class(HardwareSerial &serial, bool queryMode);
    SDS011Class(SoftwareSerial &serial, bool queryMode);
    bool begin(void);
    int readValue(float * pm25, float * pm10);
  private:
    HardwareSerial &_hwSerial;
    SoftwareSerial &_swSerial;
    bool _hardwareSerial;
    bool _queryMode;
    bool _setQueryMode(void);
    bool _setActiveMode(void);
    char * _readResponse(void);
    uint8_t _calculateChecksum(char * message, int length);
};

#endif
