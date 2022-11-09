require('./nodemanual.js')();

async function main() {
    const base_dir = "../../../datasets/";
    const [warmup_query, repeat_query] = init(base_dir);

    let twitter = []
    twitter.push("twitter_small_records_75changeUserLang.json");
    twitter.push("twitter_small_records_75deleteUser.json");
    twitter.push("twitter_small_records_75deleteUserLang.json");

    for (var dataset of twitter) {
        await execute(warmup_query, repeat_query, dataset, query_TT1, "TT1");
        await execute(warmup_query, repeat_query, dataset, query_TT2, "TT2");
        await execute(warmup_query, repeat_query, dataset, query_TT4, "TT4");
    }
}

main()