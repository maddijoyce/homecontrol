const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

global.serial;
global.callbacks = [];

function start(path, baudRate, log) {
  global.serial = new SerialPort(path, { baudRate: parseInt(baudRate, 10) });
  const lineStream = global.serial.pipe(new Readline());
  log('Serial Connection Active', path, baudRate);

  lineStream.on('data', (update) => {
    global.callbacks.forEach(element => element(update.trim()));
  });
}

function sendMessage(action) {
  global.serial.write(action + '|');
}

function addListener(listener) {
  global.callbacks.push(listener);
}

module.exports = {
  start,
  sendMessage,
  addListener,
};