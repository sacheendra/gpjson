const fs = require(`fs`);
const simdjson = require('simdjson');
const readline = require('readline');
const { performance } = require('perf_hooks');

var base_dir;
var DEBUG;

function init(dir) {
    base_dir = dir;
    const myArgs = process.argv.slice(2);
    DEBUG = myArgs.includes("DEBUG") ? true : false;
    let idx = myArgs.findIndex(v => v.includes("warmup="));
    const warmup_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 5;
    idx = myArgs.findIndex(v => v.includes("repeat="));
    const repeat_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 10;
    return [warmup_query, repeat_query];
}

// $.user.lang
async function query_TT1(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            result.push(value);
        } catch (error) {
            // console.log(error);
        }
    }
    return result;
}

// {$.user.lang, $.lang}
async function query_TT2(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            try {
                let value = jsonTape.valueForKeyPath(`user.lang`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`lang`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return result;
}

// $.user.lang[?(@ == 'nl')]"
async function query_TT3(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            if (value == "nl")
                result.push(value);
        } catch (error) {
            // console.log(error);
        }
    }
    return result;
}

// $.user.lang[?(@ == 'en')]"
async function query_TT4(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            if (value == "en")
                result.push(value);
        } catch (error) {
            // console.log(error);
        }
    }
    return result;
}

// {$.bestMarketplacePrice.price, $.name}
async function query_WM(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            try {
                let value = jsonTape.valueForKeyPath(`bestMarketplacePrice.price`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`name`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return result;
}

// $.categoryPath[1:3].id
async function query_BB(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var result = [];
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            try {
                let value = jsonTape.valueForKeyPath(`categoryPath[1].id`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`categoryPath[2].id`);
                result.push(value);
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return result;
}

async function execute(warmup_query, repeat_query, dataset, func, query) {
    if (DEBUG) console.log("Starting warmup queries on dataset " + dataset);
    var result;
    for (let i = 0; i < warmup_query; i++)
        result = await func(dataset);
    var numResults = result.length;
    var start = 0;
    var delays = [];
    for (let i = 0; i < repeat_query; i++) {
        start = performance.now();
        await func(dataset);
        delays.push(performance.now() - start);
    }
    average = delays.reduce((total, delay) => total + delay, 0) / repeat_query;
    std = Math.sqrt(delays.reduce((total, delay) => total + Math.pow((delay - average), 2), 0) / (repeat_query - 1));
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + average + "ms; results: " + numResults);
    console.log("nodesimdjson," + dataset + "," + query + "," + average + "," + std + "," + numResults + "," + warmup_query + "," + repeat_query);
}

module.exports = function() {
    this.init = init;
    this.execute = execute;
    this.query_TT1 = query_TT1;
    this.query_TT2 = query_TT2;
    this.query_TT3 = query_TT3;
    this.query_TT4 = query_TT4;
    this.query_WM = query_WM;
    this.query_BB = query_BB;
}