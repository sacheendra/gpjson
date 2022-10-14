const jp = require("jsonpath");
const fs = require("fs");
const { performance } = require('perf_hooks');

const base_dir = "../../../datasets/";

// $.user.lang
function query_TT1(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const obj = JSON.parse(file[i]);
            let value = jp.query(obj, '$.user.lang');
            count += value.length;
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
            const obj = JSON.parse(file[i]);
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
function query_TT3(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const obj = JSON.parse(file[i]);
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
function query_TT4(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const obj = JSON.parse(file[i]);
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
function query_WM(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const obj = JSON.parse(file[i]);
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
function query_BB(dataset) {
    var file = fs.readFileSync(base_dir + dataset, `utf-8`).toString().split("\n");

    var count = 0;
    for (var i = 0; i < file.length; i++) {
        try {
            const obj = JSON.parse(file[i]);
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

function execute(warmup_query, repeat_query, dataset, func, query) {
    console.log("Starting warmup queries on dataset " + dataset);
    var num_results;
    for (let i = 0; i < warmup_query; i++)
        num_results = func(dataset);
    const start = performance.now();
    for (let i = 0; i < repeat_query; i++)
        func(dataset);
    const delay = performance.now() - start;
    console.log("Executed query " + query + " on dataset " + dataset + " in " + delay / repeat_query + "ms; results: " + num_results);
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

app();