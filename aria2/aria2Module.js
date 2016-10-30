class Aria2Class {
  constructor() {
    this.child  = require('child_process')
  }

  start() {
    this.child.execFile("aria2\\aria2c", ["--enable-rpc", "--rpc-listen-all=true", "--rpc-allow-origin-all"],
      (error, stdout, stderr) => {
        console.log('stdout:', stdout)
        console.log('stderr:', stderr)
        if (error) {
          throw error
        }
    });

    console.log("Aria2c started......")
  }

  send() {

  }

  close() {
    // Shutdown
  }
};

var Aria2Module = () => {}
module.exports = Aria2Module

module.exports.Aria2Class = new Aria2Class()
