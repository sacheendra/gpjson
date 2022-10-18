require('./gpjson.js')();

function main() {
    const base_dir = "../../../datasets/";
    const [warmup_query, repeat_query] = init(base_dir);

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

main()