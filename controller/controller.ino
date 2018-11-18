#include <SPI.h>
#include <RH_NRF905.h>
#include <RHReliableDatagram.h>
#include <Radio.h>
#include <DHT.h>
#include <elapsedMillis.h>

#define DHTTYPE DHT11
#define SENSOR_WAIT 60000

RH_NRF905 nrf905(Radio::chipEnable, Radio::txEnable, Radio::slaveSelect);
RHReliableDatagram manager(nrf905, Radio::Controller);

const int sensorPin = 46;
DHT dht(sensorPin, DHTTYPE);
elapsedMillis sensorElapsed;

uint8_t buf[RADIO_MAX_PAYLOAD];

uint8_t input[RADIO_MAX_PAYLOAD + 1];
int  inputIdx = 0;
bool inputNew = false;

void setup()
{
  Serial.begin(115200);

  pinMode(Radio::power, OUTPUT);
  digitalWrite(Radio::power, HIGH);
  if (!manager.init()) Serial.println("STATUS-Controller-Failure");
  nrf905.setRF(RH_NRF905::TransmitPower10dBm);

  dht.begin();
}

void checkForMessage()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAckTimeout(buf, &len, 1000, &from))
  {
    Serial.print((char)from);
    Serial.println((char *)buf);
  }
}

void checkForCommand()
{
  while (Serial.available() > 0) {
    char i = Serial.read();
    if (i == '|' || inputIdx >= RADIO_MAX_PAYLOAD) {
      input[inputIdx] = '\0';
      inputNew = true;
      inputIdx = 0;
    } else {
      input[inputIdx++] = i;
    }
  }

  if (inputNew) {
    Serial.println((char *)input);
    uint8_t data[] = { input[1], input[2] };
    if (manager.sendtoWait(data, sizeof(data), input[0] ))
    {
      checkForMessage();
    }
    inputNew = false;
  }
}

void checkSensor() {
  if (sensorElapsed > SENSOR_WAIT) {
    float hum = dht.readHumidity();
    float tmp = dht.readTemperature();
    
    Serial.print((char)Radio::Controller);
    Serial.print((char)Radio::Value);
    Serial.print((char)Radio::Humidity);
    Serial.println(hum);

    Serial.print((char)Radio::Controller);
    Serial.print((char)Radio::Value);
    Serial.print((char)Radio::Temperature);
    Serial.println(tmp);

    sensorElapsed = 0;
  }
}

void loop()
{
  checkForCommand();

  if (manager.available())
  {
    checkForMessage();
  }

  checkSensor();
}
