const fs = require('fs');

const datasetsPath = `/home/ubuntu/datasets`;
const datasetsFilenames = {
  twitter_small: `${datasetsPath}/twitter_small_records.json`,
  twitter_smaller: `${datasetsPath}/twitter_small_records_smaller.json`,
  bestbuy_small: `${datasetsPath}/bestbuy_small_records.json`,
  walmart_small: `${datasetsPath}/walmart_small_records.json`,
}

// Define queries to use.
const q = {
    TT1: "$.user.lang",
    TT2: ["$.user.lang", "$.lang"],
    TT3: "$.user.lang[?(@ == 'nl')]",
    TT4: "$.user.lang[?(@ == 'en')]",
    WM: ["$.bestMarketplacePrice.price", "$.name"],
    BB: "$.categoryPath[1:3].id"
}

const datasetToQuery = {
    twitter_small:   [q.TT1, q.TT2, q.TT3, q.TT4],
    twitter_smaller: [q.TT1, q.TT2, q.TT3, q.TT4],
    bestbuy_small:   [q.BB],
    walmart_small:   [q.WM]
}

// Number of times to run the warmup query.
const warmupQueries = 10;

// Number of times to run every single query.
const repeatQueries = 2;

const dbsizes = {}
for (name of Object.keys(datasetsFilenames)) dbsizes[name] = fs.statSync(datasetsFilenames[name]).size;

// https://stackoverflow.com/a/18650828
function formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 bytes';
    const k = 1000;
    const dm = decimals < 0 ? 0 : decimals;
    const sizes = ['bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
}

const gpjson = Polyglot.eval('gpjson', 'jsonpath');

function queryAllOnDataset(dataset) {
    const queries = datasetToQuery[dataset];
    if (queries && queries.length > 0) {
        console.log(`-- running queries on ${dataset}`);
        for (query of queries) {
            const t0 = process.hrtime.bigint();
            for (var i = 0; i < repeatQueries; i++) gpjson.query(datasetsFilenames[dataset], query);
                const t1 = process.hrtime.bigint();

//            result = gpjson.query(datasetsFilenames[dataset], query);
//            const num_results = result[0].length;
            const num_results = "NaN";

            const timeInMs = Number(t1 - t0) / 1_000_000;
            const avgTimeInMs = timeInMs / repeatQueries;
            const avgTimeInS = avgTimeInMs / 1_000;
            const speed = dbsizes[dataset] / avgTimeInS;
            console.log(`* Query ${query} on dataset ${dataset} of size ${formatBytes(dbsizes[dataset])}: results: ${num_results} | avg time: ${avgTimeInMs}ms | speed: ${formatBytes(speed)}/s`);
        }
    }
}

function queryAllOnAllDatasets() {
    const datasets = Object.keys(datasetsFilenames);
    for (dataset of datasets) queryAllOnDataset(dataset);
}

function runWarmupQueries() {
    for (let i = 0; i < warmupQueries; i++) {
        gpjson.query(datasetsFilenames.twitter_small, q.TT1);
    }
    console.log("-- warmup done.");
}

function main() {
    console.log("-- starting script.");
    runWarmupQueries();
    queryAllOnAllDatasets();
}

main()

fs.writeFileSync('./benchmark_profile.json', gpjson.exportTimings());