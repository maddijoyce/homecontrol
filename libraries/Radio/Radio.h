#ifndef Radio_h
#define Radio_h

#include <RHReliableDatagram.h>
#include <RH_NRF905.h>
#include <SPI.h>

#define RADIO_MAX_PAYLOAD 10

class Radio {
  public:
    typedef enum {
      Controller = 'C',
      Intercom = 'I',
    } RadioDevice;

    typedef enum {
      Action = 'A',
      Status = 'S',
      Value  = 'V',
    } MessageType;
    typedef enum {
      Unlock = 'U',
      Lock = 'L',
    } MessageAction;
    typedef enum {
      IsUnlocked = 'U',
      IsLocked = 'L',
    } MessageStatus;
    typedef enum {
      Temperature = 'T',
      Humidity = 'H',
    } MessageUnit;

    static const int power       = 5;  // PWR
    static const int chipEnable  = 4;  // CE
    static const int txEnable    = 3;  // TXEN
    static const int slaveSelect = 10; // CSN

    // Also required; 3v3 to Vcc, Gnd to Gnd
    // SPI Pins Vary By Chip;
    // - Nano: MOSI to 11, MISO to 12, SCK to 13
    // - Mega: MISO to 50, MOSI to 51, SCK to 52

    // These pins don't appear to be required;
    // int _carrierDetect; // CD
    // int _rxtxReady;     // DR
    // int _addressMatch;  // AM
};

#endif
