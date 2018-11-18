#include <SPI.h>
#include <RH_NRF905.h>
#include <RHReliableDatagram.h>
#include <Radio.h>
#include <elapsedMillis.h>

RH_NRF905 nrf905(Radio::chipEnable, Radio::txEnable, Radio::slaveSelect);
RHReliableDatagram manager(nrf905, Radio::Intercom);

int relayPin = 2;
elapsedMillis timeElapsed;
uint8_t buf[RADIO_MAX_PAYLOAD];

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(Radio::power, OUTPUT);
  digitalWrite(Radio::power, HIGH);

  manager.init();
  nrf905.setRF(RH_NRF905::TransmitPower10dBm);
}

void loop() {
  if (manager.available()) {
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from)) {
      if (from == Radio::Controller && buf[0] == Radio::Action && buf[1] == Radio::Unlock) {
        digitalWrite(relayPin, HIGH);
        timeElapsed = 0;

        uint8_t data[] = { Radio::Status, Radio::IsUnlocked };
        manager.sendtoWait(data, sizeof(data), Radio::Controller);
      }
    }
  }

  if (digitalRead(relayPin) == HIGH && timeElapsed > 2000) {
    digitalWrite(relayPin, LOW);
    uint8_t data[] = { Radio::Status, Radio::IsLocked };
    manager.sendtoWait(data, sizeof(data), Radio::Controller);
  }
}

