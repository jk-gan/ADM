const electron = require('electron')
const Aria2Module = require('./aria2/API/build/Release/main')

// Module to control application life.
const app = electron.app
// Module to create native browser window.
const BrowserWindow = electron.BrowserWindow

Aria2Module.createSession((err, result) => {console.log(result);});

setTimeout(() => {Aria2Module.addUrl("https://download.lenovo.com/pccbbs/mobiles/n1mku52w.exe","https://download.lenovo.com/pccbbs/mobiles/n1mku52w.exe", (err, result) => {console.log(result);})}, 1000);

setTimeout(() => {Aria2Module.pause((err, result) => {console.log(result);})}, 5000);

setTimeout(() => {Aria2Module.killSession((err, result) => {console.log(result);})}, 10000);

// Initiallize aria class

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

function createWindow () {
  // Create the browser window.
  mainWindow = new BrowserWindow({width: 800, height: 600})

  // and load the index.html of the app.
  mainWindow.loadURL(`file://${__dirname}/index.html`)

  // Open the DevTools.
  mainWindow.webContents.openDevTools()

  // Emitted when the window is closed.
  mainWindow.on('close', (e) => {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow.hide();

    app.quit()

  })

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
app.on('ready', () => {
  createWindow();
})

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
