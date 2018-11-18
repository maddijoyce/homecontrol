const { addListener, sendMessage } = require('./serial');

var Service, Characteristic

let humidity = null;
let temperature = null;

function Sensor(config, log, hGlobals) {
  Service = hGlobals.Service;
  Characteristic = hGlobals.Characteristic;

  this.name = config["name"];
  this.humidityService = new Service.HumiditySensor('Humidity');
  this.temperatureService = new Service.TemperatureSensor('Temperature');
  
  this.humidityService
    .getCharacteristic(Characteristic.CurrentRelativeHumidity)
    .on('get', (callback) => humidity ? callback(null, humidity) : 'Not Available');
  this.temperatureService
    .getCharacteristic(Characteristic.CurrentTemperature)
    .on('get', (callback) => temperature ? callback(null, temperature) : 'Not Available');

  addListener((text) => {
    if (text.indexOf('CVH') === 0) {
      humidity = parseFloat(text.replace('CVH', ''));
      this.humidityService.setCharacteristic(Characteristic.CurrentRelativeHumidity, humidity);
    }
    if (text.indexOf('CVT') === 0) {
      temperature = parseFloat(text.replace('CVT', ''));
      this.temperatureService.setCharacteristic(Characteristic.CurrentTemperature, temperature);
    }
  });
}

Sensor.prototype.getServices = function() {
  return [this.humidityService, this.temperatureService];
}

module.exports = Sensor;