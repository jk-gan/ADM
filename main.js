const electron = require('electron')

// Module to control application life.
// Module to create native browser window.
const { app, BrowserWindow } = electron

const path = require('path')
const url = require('url')

// const ipcMain = electron.ipcMain;
// const Aria2Module = require('./aria2/API/build/Release/main')

// global.Aria2Module = Aria2Module;

// setTimeout(() => {
//   Aria2Module.createSession(1, (err, result) => {
//     Aria2Module.addUrl(result, "https://download.lenovo.com/pccbbs/mobiles/n1mku52w.exe","https://download.lenovo.com/pccbbs/mobiles/n1mku52w.exe", (err, result) => {console.log(result);})
//   });
//   Aria2Module.createSession(2, (err, result) => {
//     Aria2Module.addUrl(result, "http://files2.codecguide.com/K-Lite_Codec_Pack_1290_Mega.exe","http://files2.codecguide.com/K-Lite_Codec_Pack_1290_Mega.exe", (err, result) => {console.log(result);})
//   });
// }, 1000);

//setTimeout(() => {Aria2Module.pause((err, result) => {console.log(result);})}, 5000);

//setTimeout(() => {Aria2Module.killSession((err, result) => {console.log(result);})}, 10000);

// Initiallize aria class

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({ width: 800, height: 600 })

  // and load the index.html of the app.
  mainWindow.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true,
  }))

  // Open the DevTools.
  mainWindow.webContents.openDevTools()

  // // Emitted when the window is closed.
  // mainWindow.on('close', (e) => {
  //   // Dereference the window object, usually you would store windows
  //   // in an array if your app supports multi windows, this is the time
  //   // when you should delete the corresponding element.
  //   mainWindow.hide();

  //   // save data and quit
  //   Aria2Module.killAllSession((err, result) => {console.log(result);app.quit();});
  // })

  mainWindow.on('closed', (e) => {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.

    mainWindow = null
  })
}

app.on('before-quit', () => {
  mainWindow.forceClose = true;
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)

// Quit when all windows are closed.
app.on('window-all-closed', function () {
  // On OS X it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', function () {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
    createWindow()
  }
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
