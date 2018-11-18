const { addListener, sendMessage } = require('./serial');

var Service, Characteristic
let isLocked = true;

function Intercom(config, log, hGlobals) {
  Service = hGlobals.Service;
  Characteristic = hGlobals.Characteristic;

  this.log = log;
  this.name = config["name"];
  this.service = new Service.LockMechanism(this.name);
  
  this.service.getCharacteristic(Characteristic.LockCurrentState)
    .on('get', (callback) => callback(null, isLocked));
  
  this.service.getCharacteristic(Characteristic.LockTargetState)
    .on('get', (callback) => callback(null, isLocked))
    .on('set', this.setState.bind(this));

  addListener((text) => {
    if (text.indexOf('ISU') === 0) {
      isLocked = false;
      this.service.setCharacteristic(Characteristic.LockCurrentState, Characteristic.LockCurrentState.UNSECURED);
    }
    if (text.indexOf('ISL') === 0) {
      isLocked = true;
      this.service.setCharacteristic(Characteristic.LockTargetState, Characteristic.LockTargetState.SECURED);
      this.service.setCharacteristic(Characteristic.LockCurrentState, Characteristic.LockCurrentState.SECURED);
    }
  });
}

Intercom.prototype.setState = function(state, callback) {
  if (state === Characteristic.LockTargetState.UNSECURED) {
    sendMessage('IAU');
  }
  callback(null);
}

Intercom.prototype.getServices = function() {
  return [this.service];
}

module.exports = Intercom;