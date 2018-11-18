const { start } = require('./serial');
const Intercom = require('./intercom');
const Sensor = require('./sensor');

const hbGlobal = {};

module.exports = function(homebridge) {
  console.log('Homebridge API Version: ' + homebridge.version);

  hbGlobal.Acessory = homebridge.platformAccessory;
  hbGlobal.Service = homebridge.hap.Service;
  hbGlobal.Characteristic = homebridge.hap.Characteristic;

  homebridge.registerPlatform('homebridge-maroubra', 'Maroubra', Maroubra);
}

// const intercomUUID = 'dc9d785b-407c-42cd-8391-6630511e08e8';
// const mainSensorUUID = 'd46aa21a-d11a-447e-8e3b-92b395c744cc';

function Maroubra(log, config) {
  Platform = this;
  this.log = log;
  this.config = config;

  try {
    start(config.serialPath, config.baudRate, log);
  } catch (e) {
    log("Didn't start serial connection", e);
  }
}

Maroubra.prototype = {
  accessories: function(callback) {
    const all = [
      new Intercom({ name: 'Intercom' }, this.log, hbGlobal),
      new Sensor({ name: 'Main Room Sensor' }, this.log, hbGlobal),
    ];
    callback(all);
  },
};