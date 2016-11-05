class Aria2Class {

  constructor() {
    this.exec  = require('child_process').exec
    this.WebSocket = require('ws');
    this.isClosed = false;

    // list of command
    this.startCommand = "aria2\\aria2c --enable-rpc --rpc-listen-all=true --rpc-allow-origin-all";
  }

  start() {
    this.child = this.exec(this.startCommand)

    let socketInterval = setInterval(() => {
      if(this.child){
        this.socket = new this.WebSocket('ws://localhost:6800/jsonrpc')

        // for debuging
        this.socket.on('message', (message) => {
          let messageObj = JSON.parse(message)
          console.log(messageObj);
        });

        this.socket.on('close', () => {
          console.log("close")
        });

        this.socket.on('open', () => {
          console.log("open")
        });

        clearInterval(socketInterval);
      }
    }, 1000)

    // for debuging
    this.child.stdout.on('data', (data) => {
                      console.log(data.toString());
                    });

    this.child.on('exit', () => {
      this.socket.close()
      this.isClosed = true;
    })
  }

  close() {
    // Shutdown
    let methodJson = {'json-rpc': '2.0',
    'id': 'qwer',
    'method': 'aria2.shutdown'}

    this.socket.send(JSON.stringify(methodJson))
  }
};

var Aria2Module = () => {}
module.exports = Aria2Module

module.exports.Aria2Class = new Aria2Class()
