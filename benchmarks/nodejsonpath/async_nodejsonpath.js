const jp = require("jsonpath");
const fs = require("fs");
const readline = require('readline');
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";

// $.user.lang
async function query_TT1(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});
    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            let value = jp.query(obj, '$.user.lang');
            count += value.length;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.user.lang, $.lang}
async function query_TT2(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            try {
                let value = jp.query(obj, '$.user.lang');
                count += value.length;
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jp.query(obj, '$.lang');
                count += value.length;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return count;
}

// $.user.lang[?(@ == 'nl')]"
async function query_TT3(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            let value = jp.query(obj, '$.user.lang');
            if (value == "nl")
                count += value.length;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// $.user.lang[?(@ == 'en')]"
async function query_TT4(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            let value = jp.query(obj, '$.user.lang');
            if (value == "en")
                count += value.length;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.bestMarketplacePrice.price, $.name}
async function query_WM(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            try {
                let value = jp.query(obj, '$.bestMarketplacePrice.price');
                count += value.length;
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jp.query(obj, '$.name');
                count += value.length;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return count;
}

// $.categoryPath[1:3].id
async function query_BB(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            try {
                let value = jp.query(obj, '$.categoryPath[1:3].id');
                count += value.length;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return count;
}


async function execute(warmup_query, repeat_query, dataset, func, query) {
    if (DEBUG) console.log("Starting warmup queries on dataset " + dataset);
    var num_results;
    for (let i = 0; i < warmup_query; i++)
        num_results = await func(dataset);
    const start = performance.now();
    for (let i = 0; i < repeat_query; i++)
        await func(dataset);
    const delay = performance.now() - start;
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms; results: " + num_results);
    console.log("nodejsonpath,"+dataset+","+query+","+delay/repeat_query+","+num_results+","+warmup_query+","+repeat_query);
}

async function app() {
    let idx = myArgs.findIndex(v => v.includes("warmup="));
    const warmup_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 5;
    idx = myArgs.findIndex(v => v.includes("repeat="));
    const repeat_query = idx >= 0 ? parseInt(myArgs[idx].substring(7)) : 10;

    const twitter_small = "twitter_small_records.json";
    const walmart_small = "walmart_small_records.json";
    const bestbuy_small = "bestbuy_small_records.json";

    await execute(warmup_query, repeat_query, twitter_small, query_TT1, "TT1");
    await execute(warmup_query, repeat_query, twitter_small, query_TT2, "TT2");
    await execute(warmup_query, repeat_query, twitter_small, query_TT3, "TT3");
    await execute(warmup_query, repeat_query, twitter_small, query_TT4, "TT4");

    await execute(warmup_query, repeat_query, walmart_small, query_WM, "WM");

    await execute(warmup_query, repeat_query, bestbuy_small, query_BB, "BB");
}

const myArgs = process.argv.slice(2);
DEBUG = myArgs.includes("DEBUG") ? true : false;
app();