const fs = require("fs");
const readline = require('readline');
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";

// $.user.lang
async function query_TT1(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "user") {
                    for (var key2 in obj[key1]) {
                        if (obj[key1].hasOwnProperty(key2) && key2 == "lang") {
                            value = obj[key1][key2];
                            count += 1;
                        }
                    }
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.user.lang, $.lang}
async function query_TT2(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "user") {
                    for (var key2 in obj[key1]) {
                        if (obj[key1].hasOwnProperty(key2) && key2 == "lang") {
                            value = obj[key1][key2];
                            count += 1;
                        }
                    }
                }
                if (obj.hasOwnProperty(key1) && key1 == "lang") {
                    value = obj[key1];
                    count += 1;
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// $.user.lang[?(@ == 'nl')]"
async function query_TT3(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "user") {
                    for (var key2 in obj[key1]) {
                        if (obj[key1].hasOwnProperty(key2) && key2 == "lang" && obj[key1][key2] == "nl") {
                            value = obj[key1][key2];
                            count += 1;
                        }
                    }
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// $.user.lang[?(@ == 'en')]"
async function query_TT4(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "user") {
                    for (var key2 in obj[key1]) {
                        if (obj[key1].hasOwnProperty(key2) && key2 == "lang" && obj[key1][key2] == "en") {
                            value = obj[key1][key2];
                            count += 1;
                        }
                    }
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// {$.bestMarketplacePrice.price, $.name}
async function query_WM(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "bestMarketplacePrice") {
                    for (var key2 in obj[key1]) {
                        if (obj[key1].hasOwnProperty(key2) && key2 == "price") {
                            value = obj[key1][key2];
                            count += 1;
                        }
                    }
                }
                if (obj.hasOwnProperty(key1) && key1 == "name") {
                    value = obj[key1];
                    count += 1;
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

// $.categoryPath[1:3].id
async function query_BB(dataset) {
    var lineReader = readline.createInterface({ input: fs.createReadStream(base_dir + dataset) });

    var count = 0;
    for await (const line of lineReader) {
        try {
            const obj = JSON.parse(line);
            for (var key1 in obj) {
                if (obj.hasOwnProperty(key1) && key1 == "categoryPath") {
                    for (var j = 1; j < 3 && j < obj[key1].length; j++) {
                        for (var key2 in obj[key1][j]) {
                            if (obj[key1][j].hasOwnProperty(key2) && key2 == "id") {
                                value = obj[key1][j][key2];
                                count += 1;
                            }
                        }
                    }
                }
            }
        } catch (error) {
            // console.log(error);
        }
    }
    return count;
}

async function execute(warmup_query, repeat_query, dataset, func, query) {
    if (DEBUG) console.log("Starting warmup queries on dataset " + dataset);
    var num_results;
    for (let i = 0; i < warmup_query; i++)
        num_results = await func(dataset);
    var start = 0;
    var delays = [];
    for (let i = 0; i < repeat_query; i++) {
        start = performance.now();
        await func(dataset);
        delays.push(performance.now() - start);
    }
    average = delays.reduce((total, delay) => total + delay, 0) / repeat_query;
    std = Math.sqrt(delays.reduce((total, delay) => total + Math.pow((delay - average), 2), 0) / (repeat_query - 1));
    if (DEBUG) console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms; results: " + num_results);
    console.log("nodemanual," + dataset + "," + query + "," + average + "," + std + "," + num_results + "," + warmup_query + "," + repeat_query);
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