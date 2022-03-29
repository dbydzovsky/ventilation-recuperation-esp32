module.exports = function override(config, env) {
    config.optimization.splitChunks = {
        cacheGroups: {
            default: false,
        },
    };
    config.optimization.runtimeChunk = false;
    config.output = {
        ...config.output, // copy all settings
        filename: "js/[hash].js",
        // chunkFilename: "static/js/[name].[hash].chunk.js",
    };
    return config;
};
