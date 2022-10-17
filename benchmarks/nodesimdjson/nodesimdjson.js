const fs = require(`fs`);
const simdjson = require('simdjson');
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";

// $.user.lang
function query_TT1(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            count++;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.user.lang, $.lang}
function query_TT2(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            try {
                let value = jsonTape.valueForKeyPath(`user.lang`);
                count++;
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`lang`);
                count++;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return count;
}

// $.user.lang[?(@ == 'nl')]"
function query_TT3(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            if (value == "nl")
                count++;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// $.user.lang[?(@ == 'en')]"
function query_TT4(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            if (value == "en")
                count++;
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.bestMarketplacePrice.price, $.name}
function query_WM(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            try {
                let value = jsonTape.valueForKeyPath(`bestMarketplacePrice.price`);
                count++;
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`name`);
                count++;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
    }
    return count;
}

// $.categoryPath[1:3].id
function query_BB(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const jsonTape = simdjson.lazyParse(file[i]);
            try {
                let value = jsonTape.valueForKeyPath(`categoryPath[1].id`);
                count++;
            } catch (error) {
                // console.log(error);
            }
            try {
                let value = jsonTape.valueForKeyPath(`categoryPath[2].id`);
                count++;
            } catch (error) {
                // console.log(error);
            }
        } catch { }
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
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms; results: " + num_results);
    console.log("nodesimdjson,"+dataset+","+query+","+delay/repeat_query+","+num_results+","+warmup_query+","+repeat_query);
}

function app() {
    const warmup_query = 5;
    const repeat_query = 10;

    const twitter_small = "twitter_small_records.json";
    const walmart_small = "walmart_small_records.json";
    const bestbuy_small = "bestbuy_small_records.json";

    execute(warmup_query, repeat_query, twitter_small, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_small, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_small, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_small, query_TT4, "TT4");

    execute(warmup_query, repeat_query, walmart_small, query_WM, "WM");

    execute(warmup_query, repeat_query, bestbuy_small, query_BB, "BB");
}

const myArgs = process.argv.slice(2);
DEBUG = myArgs.includes("DEBUG") ? true : false;
app();

// ### Not lazy version ###
// console.time("json parse and query");
// var count = 0;
// for (var i = 0; i < file.length; i++) {
//     try {
//         const jsonTape = simdjson.parse(file[i]);
//         let value = jsonTape["user"]["lang"];
//         count++;
//     } catch (error) {
//         // console.log(error);
//     }
// }
// console.timeEnd("json parse and query");
// console.log(count);