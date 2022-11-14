require('./nodejsonpathplus.js')();

async function main() {
    const base_dir = "../../../datasets/";
    const [warmup_query, repeat_query] = init(base_dir);

    let twitter = []
    twitter.push("twitter_small_records_0changeUserLang.json");
    twitter.push("twitter_small_records_0deleteUser.json");
    twitter.push("twitter_small_records_0deleteUserLang.json");
    twitter.push("twitter_small_records_25changeUserLang.json");
    twitter.push("twitter_small_records_25deleteUser.json");
    twitter.push("twitter_small_records_25deleteUserLang.json");
    twitter.push("twitter_small_records_50changeUserLang.json");
    twitter.push("twitter_small_records_50deleteUser.json");
    twitter.push("twitter_small_records_50deleteUserLang.json");
    twitter.push("twitter_small_records_75changeUserLang.json");
    twitter.push("twitter_small_records_75deleteUser.json");
    twitter.push("twitter_small_records_75deleteUserLang.json");
    twitter.push("twitter_small_records_100changeUserLang.json");
    twitter.push("twitter_small_records_100deleteUser.json");
    twitter.push("twitter_small_records_100deleteUserLang.json");

    for (var dataset of twitter) {
        await execute(warmup_query, repeat_query, dataset, query_TT1, "TT1");
        await execute(warmup_query, repeat_query, dataset, query_TT2, "TT2");
        await execute(warmup_query, repeat_query, dataset, query_TT3, "TT3");
        await execute(warmup_query, repeat_query, dataset, query_TT4, "TT4");
    }
}

main()