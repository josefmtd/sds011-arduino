#include "SDS011.h"

SDS011Class::SDS011Class(HardwareSerial& serial, bool queryMode) :
  _hwSerial(serial),
  _queryMode(queryMode) {
    _serial = _hwSerial;
}

SDS011Class::SDS011Class(SoftwareSerial& serial, bool queryMode) :
  _swSerial(serial),
  _queryMode(queryMode) {
    _serial = _swSerial;
}


static const char setModeActive[19] = {
  0xAA, 0xB4, 0x02, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00 ,0x00,
  0xFF, 0xFF, 0x01, 0xAB
};

static const char setModeQuery[19] = {
  0xAA, 0xB4, 0x02, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x02, 0xAB
};

static const char sendQuery[19] = {
  0xAA, 0xB4, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x02, 0xAB
};

bool SDS011Class::begin(void) {
  _serial.begin(9600, SERIAL_8N1);
  if (_queryMode) {
    _setQueryMode();
  }
  else {
    _setActiveMode();
  }
}

bool SDS011Class::_setActiveMode() {
  bool retValue;

  for (int i = 0; i < COMMAND_SIZE; i++) {
    _serial.write(setModeActive[i]);
  }
  _serial.flush();

  char * response = _readResponse();

  uint8_t checkSum = _calculateChecksum(response + SENSOR_DATA1, 6);

  if ((uint8_t) response[SENSOR_COMMAND == ID_RESPONSE] && checkSum == (uint8_t)response[SENSOR_CHECKSUM] ) {
    retValue = (response[SENSOR_DATA1] == setModeQuery[HOST_DATA1] &&
                response[SENSOR_DATA2] == setModeQuery[HOST_DATA2] &&
                response[SENSOR_DATA3] == setModeQuery[HOST_DATA3]);
  }
  else {
    retValue = false;
  }

  free(response);
  return retValue;
}

bool SDS011Class::_setQueryMode() {
  bool retValue;
  
  for (int i = 0; i < COMMAND_SIZE; i++) {
    _serial.write(setModeQuery[i]);
  }
  _serial.flush();

  char * response = _readResponse();

  uint8_t checkSum = _calculateChecksum(response + SENSOR_DATA1, 6);

  if (response[SENSOR_COMMAND == ID_RESPONSE] && checkSum == (uint8_t)response[SENSOR_CHECKSUM] ) {
    retValue = (response[SENSOR_DATA1] == setModeQuery[HOST_DATA1] &&
                response[SENSOR_DATA2] == setModeQuery[HOST_DATA2] &&
                response[SENSOR_DATA3] == setModeQuery[HOST_DATA3]);
  }
  else {
    retValue = false;
  }

  free(response);
  return retValue;
}

char * SDS011Class::_readResponse(void) {
  char * response = (char *)malloc(sizeof(char)*RESPONSE_SIZE);
  int i = 0;

  while (_serial.available() > 0) {
    response[i] = _serial.read();
    i++;

    if (i == RESPONSE_SIZE) {
      break;
    }
  }

  return response;
}

uint8_t SDS011Class::_calculateChecksum(char * message, int length) {
  uint8_t checkSum = 0;

  for (int i = 0; i < length; i++) {
    checkSum += (uint8_t)message[i];
  }

  checkSum &= 255;

  return checkSum;
}

int SDS011Class::readValue(float * pm25, float * pm10) {
  if (_queryMode) {
    for (int i = 0; i < COMMAND_SIZE; i++) {
      _serial.write(sendQuery[i]);
    }
    _serial.flush();
  }
  
  char * response = _readResponse();
  uint8_t test = response[SENSOR_COMMAND];
  uint8_t idValue = ID_VALUE;

  uint8_t checkSum = _calculateChecksum(response + SENSOR_DATA1, 6);
  
  if ((uint8_t) response[SENSOR_COMMAND] == ID_VALUE && checkSum == (uint8_t)response[SENSOR_CHECKSUM]) {
    uint16_t readingPM25 = ( (uint16_t)response[SENSOR_DATA2] * 256 ) + (uint16_t)response[SENSOR_DATA1];
    uint16_t readingPM10 = ( (uint16_t)response[SENSOR_DATA4] * 256 ) + (uint16_t)response[SENSOR_DATA3];

    *pm25 = (float)readingPM25/10.0;
    *pm10 = (float)readingPM10/10.0;

    free(response);
    return 0;
  }
  else {
    free(response);
    return -1;
  }
}
