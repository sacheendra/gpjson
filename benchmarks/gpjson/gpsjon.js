const fs = require('fs');
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";
var gpjson;

// https://stackoverflow.com/a/18650828
function formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 bytes';
    const k = 1000;
    const dm = decimals < 0 ? 0 : decimals;
    const sizes = ['bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
}

// $.user.lang
function query_TT1(dataset) {
    var count = 0;
    var result = gpjson.query(base_dir + dataset, "$.user.lang");
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    return count;
}

// {$.user.lang, $.lang}
function query_TT2(dataset) {
    var count = 0;
    result = gpjson.query(base_dir + dataset, ["$.user.lang", "$.lang"]);
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    for (let i = 0; i < result[1].length; i++) {
        for (let j = 0; j < result[1][i].length; j++) {
            if (result[1][i][j] != null)
                count += 1;
        }
    }
    return count;
}

// $.user.lang[?(@ == 'nl')]
function query_TT3(dataset) {
    var count = 0;
    result = gpjson.query(base_dir + dataset, "$.user.lang[?(@ == 'nl')]");
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    return count;
}

// $.user.lang[?(@ == 'en')]
function query_TT4(dataset) {
    var count = 0;
    result = gpjson.query(base_dir + dataset, "$.user.lang[?(@ == 'en')]");
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    return count;
}

// {$.bestMarketplacePrice.price, $.name}
function query_WM(dataset) {
    var count = 0;
    result = gpjson.query(base_dir + dataset, ["$.bestMarketplacePrice.price", "$.name"]);
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    for (let i = 0; i < result[1].length; i++) {
        for (let j = 0; j < result[1][i].length; j++) {
            if (result[1][i][j] != null)
                count += 1;
        }
    }
    return count;
}

// $.categoryPath[1:3].id
function query_BB(dataset) {
    var count = 0;
    result = gpjson.query(base_dir + dataset, "$.categoryPath[1:3].id");
    for (let i = 0; i < result[0].length; i++) {
        for (let j = 0; j < result[0][i].length; j++) {
            if (result[0][i][j] != null)
                count += 1;
        }
    }
    return count;
}

function execute(warmup_query, repeat_query, dataset, func, query) {
    if (DEBUG) console.log("Starting warmup queries on dataset " + dataset);
    var num_results;
    for (let i = 0; i < warmup_query; i++)
        num_results = func(dataset);
    const start = performance.now();
    for (let i = 0; i < repeat_query; i++)
        func(dataset);
    const delay = performance.now() - start;
    const speed = fs.statSync(base_dir + dataset).size / delay * 1000;
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms | " + formatBytes(speed) + "/s; results: " + num_results);
    console.log("gpjson,"+dataset+","+query+","+delay/repeat_query+","+num_results+","+warmup_query+","+repeat_query);
}

function main() {
    const warmup_query = 5;
    const repeat_query = 10;

    const test_small = "test_small_records.json";
    const twitter_small = "twitter_small_records.json";
    const walmart_small = "walmart_small_records.json";
    const bestbuy_small = "bestbuy_small_records.json";

    gpjson = Polyglot.eval('gpjson', 'jsonpath');

    execute(warmup_query, repeat_query, twitter_small, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_small, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_small, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_small, query_TT4, "TT4");

    execute(warmup_query, repeat_query, walmart_small, query_WM, "WM");

    execute(warmup_query, repeat_query, bestbuy_small, query_BB, "BB");
}

const myArgs = process.argv.slice(2);
DEBUG = myArgs.includes("DEBUG") ? true : false;
main()

// fs.writeFileSync('./benchmark_profile.json', gpjson.exportTimings());