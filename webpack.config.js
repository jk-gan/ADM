module.exports = {
    entry: "./app/app.js",
    output: {
        path: __dirname + "/public/js",
        filename: "bundle.js"
    },
    target : 'electron',
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
