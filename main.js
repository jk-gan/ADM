const electron = require('electron');

// Module to control application life.
// Module to create native browser window.
const { app, BrowserWindow, dialog, Menu, MenuItem } = electron;

const path = require('path');
const url = require('url');
const fs = require('fs');
const prompt = require('electron-prompt');

const Aria2Module = require('./build/Release/main');

// Initialize Aria2
Aria2Module.ariaInit();

// Construct right click menu
const menu = new Menu();

global.Aria2Module = Aria2Module;
global.fs = fs;
global.dialog = dialog;
global.MenuItem = MenuItem;
global.menu = menu;
global.prompt = prompt;

let mainWindow;

app.disableHardwareAcceleration();

function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({ width: 1000, height: 700 });

  // and load the index.html of the app.
  mainWindow.loadURL(
    url.format({
      pathname: path.join(__dirname, 'index.html'),
      protocol: 'file:',
      slashes: true,
    })
  );

  // Open the DevTools.
  if (!app.isPackaged) {
    //mainWindow.webContents.openDevTools();
  }

  // // Emitted when the window is closed.
  mainWindow.on('close', e => {
    // save data and quit
    Aria2Module.stopMonitoring();
    Aria2Module.killAllSession();
    Aria2Module.ariaDeInit();
    app.quit();
  });

  mainWindow.on('closed', e => {
    mainWindow = null;
  });
}

app.on('before-quit', () => {
  mainWindow.forceClose = true;
});

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});
