#Aria2 Download Manager

This is a download manager is built with [electron](http://electron.atom.io) and [aria2](https://aria2.github.io).

## To Use

To clone and run this repository you'll need [Git](https://git-scm.com) and [Node.js](https://nodejs.org/en/download/) (which comes with [npm](http://npmjs.com)) installed on your computer. From your command line:

```bash
# Clone this repository
git clone git@github.com:jk-gan/ADM.git
# Go into the repository
cd ADM
# Install dependencies and run the app
npm install -g browserify 
sudo npm install -g watchify
npm install && npm start
```

## Directory Structure
```
|---app #All React projects goes here
|----components # Presentation Component Directory
|----app.js
|---public # Client Files here
|----css
|----js
|------bundle.js
|---index.html # Electron Default View
|---main.js # Electron entry point
|---package.json
```

## Before start the app
```bash
# Use browserify to bundle up js file into single bundle.js
# Everytime there is a new change
browserify -t [ babelify ] app/app.jsx -o public/js/bundle.js 

# or you can use watchify - just waites for change and re-creates the bundle
npm run watch
```

## Start the app
```bash
npm start
```

Learn more about Electron and its API in the [documentation](http://electron.atom.io/docs/latest).

#### License [MIT](LICENSE.md)