module.exports = {
    entry: "./src/index.tsx",
    output: {
        filename: "bundle.js",
        path: __dirname + "/dist"
    },
    // enable sourcemaps for debugging webpack's output 
    devtool: "source-map",

    resolve: {
        // add .ts and .tsx as resolvable extensions 
        extensions: [".ts", ".tsx", ".js", ".json"]
    },
    module: {
        rules: [
            // all files with `.ts` and `.tsx` will be handled by `awesome-typescript-loader` 
            { test: /\.tsx?$/, loader: "awesome-typescript-loader" },
            
            // all output `.js` files will have any sourcemaps re-processed by `source-map-loader`
            { enforce: "pre", test: /\.js$/, loader: "source-map-loader" }
        ]
    },

    // when importing module whose path matches, just assume 
    // corresponding global variable exists and use that instead 
    // this is important because it allows us to avoid bundling all 
    // of dependencies, which allows browsers to cache libraries between builds
    externals: {
        "react": "React",
        "react-dom": "ReactDOM"
    },
};
