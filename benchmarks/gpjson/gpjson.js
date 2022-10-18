import { statSync } from 'fs';
import { performance } from 'perf_hooks';

var base_dir;
var gpjson;
var DEBUG;

// https://stackoverflow.com/a/18650828
function formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 bytes';
    const k = 1000;
    const dm = decimals < 0 ? 0 : decimals;
    const sizes = ['bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
}

function init(dir) {
    base_dir = dir;
    gpjson = Polyglot.eval('gpjson', 'jsonpath');
    const myArgs = process.argv.slice(2);
    DEBUG = myArgs.includes("DEBUG") ? true : false;
    let idx = myArgs.findIndex(v => v.includes("warmup="));
    const warmup_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 5;
    idx = myArgs.findIndex(v => v.includes("repeat="));
    const repeat_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 10;
    return [warmup_query, repeat_query];
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
    var start = 0;
    var delays = [];
    for (let i = 0; i < repeat_query; i++) {
        start = performance.now();
        func(dataset);
        delays.push(performance.now() - start);
    }
    average = delays.reduce((total, delay) => total + delay, 0) / repeat_query;
    std = Math.sqrt(delays.reduce((total, delay) => total + Math.pow((delay - average), 2), 0) / (repeat_query - 1));
    const speed = statSync(base_dir + dataset).size / average * 1000;
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + average + "ms | " + formatBytes(speed) + "/s; results: " + num_results);
    console.log("gpjson," + dataset + "," + query + "," + average + "," + std + "," + num_results + "," + warmup_query + "," + repeat_query);
}

export const init = init;
export const execute = execute;
export const query_TT1 = query_TT1;
export const query_TT2 = query_TT2;
export const query_TT3 = query_TT3;
export const query_TT4 = query_TT4;
export const query_WM = query_WM;
export const query_BB = query_BB;

// fs.writeFileSync('./benchmark_profile.json', gpjson.exportTimings());