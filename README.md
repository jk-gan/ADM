#Aria2 Download Manager

This is a download manager is built with [electron](http://electron.atom.io), [react](https://facebook.github.io/react/) and [aria2](https://aria2.github.io).

## To Use

To clone and run this repository you'll need [Git](https://git-scm.com) and [Node.js](https://nodejs.org/en/download/) (which comes with [npm](http://npmjs.com)) installed on your computer. From your command line:

```bash
# Clone this repository
git clone git@github.com:jk-gan/ADM.git
# Go into the repository
cd ADM
# Install dependencies and run the app
npm install && npm start
```

## Directory Structure
```
|---app #All React projects goes here
|----components # Presentation Component Directory
|----containers # Container Component Directory
|----app.js
|---public # Client Files here
|----js
|------bundle.js # The bundle file by webpack
|---index.html # Electron Default View
|---main.js # Electron entry point
|---package.json
|---webpack.config.js
```

## Before start the app
```bash
# Use webpack to bundle up js file into single bundle.js
# Everytime there is a new change
webpack

# or you can use watchify - just waites for change and re-creates the bundle
npm run watch
```

## Start the app
```bash
npm start
```

Learn more about Electron and its API in the [documentation](http://electron.atom.io/docs/latest).

#### License [MIT](LICENSE.md)
