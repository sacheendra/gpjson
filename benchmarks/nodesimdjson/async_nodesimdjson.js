const fs = require(`fs`);
const simdjson = require('simdjson');
const readline = require('readline');
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";

// $.user.lang
async function query_TT1(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
            let value = jsonTape.valueForKeyPath(`user.lang`);
            count++;
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
            const jsonTape = simdjson.lazyParse(line);
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
async function query_TT3(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
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
async function query_TT4(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
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
async function query_WM(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
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
async function query_BB(dataset) {
    var lineReader = readline.createInterface({input: fs.createReadStream(base_dir + dataset)});

    var count = 0;
    for await (const line of lineReader) {
        try {
            const jsonTape = simdjson.lazyParse(line);
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

async function execute(warmup_query, repeat_query, dataset, func, query) {
    console.log("Starting warmup queries on dataset " + dataset);
    var num_results;
    for (let i = 0; i < warmup_query; i++)
        num_results = await func(dataset);
    const start = performance.now();
    for (let i = 0; i < repeat_query; i++)
        await func(dataset);
    const delay = performance.now() - start;
    console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms; results: " + num_results);
}

async function app() {
    const warmup_query = 5;
    const repeat_query = 10;

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