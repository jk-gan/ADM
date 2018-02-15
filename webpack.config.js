const webpack = require('webpack')
const path = require('path');

module.exports = {
  entry: "./src/app.js",
  output: {
    path: path.join(__dirname, "/dist/js"),
    filename: "bundle.js"
  },
  target: 'electron',
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: 'babel-loader'
      }
    ]
  }
};
