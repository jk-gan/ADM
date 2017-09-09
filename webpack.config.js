module.exports = {
    entry: "./app/app.js",
    output: {
        path: __dirname + "/public/js",
        filename: "bundle.js"
    },
    target : 'electron',
    module: {
        loaders: [
            {
              test: /\.js$/,
              exclude: /(node_modules|bower_components)/,
              loader: 'babel',
              query: {
                presets: [
                  'es2015',
                  'react'
                ]
              }
            }
        ]
    }
};
