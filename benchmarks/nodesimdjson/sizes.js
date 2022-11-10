require('./nodesimdjson.js')();

async function main() {
    const base_dir = "../../../datasets/";
    const [warmup_query, repeat_query] = init(base_dir);

    let twitter = []
    twitter.push("twitter_small_records.json");
    twitter.push("twitter_small_records_0.125x.json");
    twitter.push("twitter_small_records_0.25x.json");
    twitter.push("twitter_small_records_0.5x.json");
    twitter.push("twitter_small_records_1.5x.json");
    twitter.push("twitter_small_records_2x.json");
    twitter.push("twitter_small_records_2.5x.json");

    for (var dataset of twitter) {
        await execute(warmup_query, repeat_query, dataset, query_TT1, "TT1");
        await execute(warmup_query, repeat_query, dataset, query_TT2, "TT2");
        await execute(warmup_query, repeat_query, dataset, query_TT3, "TT3");
        await execute(warmup_query, repeat_query, dataset, query_TT4, "TT4");
    }
}

main()