class Aria2Class {

  constructor() {
    this.exec  = require('child_process').exec
    this.WebSocket = require('ws');
    this.isClosed = false;

    // list of command
    this.startCommand = "aria2\\aria2c --enable-rpc --rpc-listen-all=true --rpc-allow-origin-all";
  }

  start() {
    this.child = this.exec(this.startCommand);
    this.socket = new this.WebSocket('ws://localhost:6800/jsonrpc')

    // for debuging
    this.child.stdout.on('data', (data) => {
                      console.log(data.toString());
                    });

    // for debuging
    this.socket.on('message', (message) => {
      let messageObj = JSON.parse(message)
      console.log(messageObj);
    });

    this.child.on('exit', () => {
      this.isClosed = true;
    })

    this.socket.on('close', () => {
      console.log("close")
    });

    this.socket.on('open', () => {
      console.log("open")
    });
  }

  close() {
    // Shutdown
    let methodJson = {'json-rpc': '2.0',
    'id': 'qwer',
    'method': 'aria2.shutdown'}

    this.socket.close(this.socket.send(JSON.stringify(methodJson)));
  }
};

var Aria2Module = () => {}
module.exports = Aria2Module

module.exports.Aria2Class = new Aria2Class()
