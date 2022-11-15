require('./nodemanual.js')();

async function main() {
    const base_dir = "../../../datasets/";
    const [warmup_query, repeat_query] = init(base_dir);

    let twitter = []
    twitter.push("twitter_small_records_changeUserLang0.json");
    twitter.push("twitter_small_records_deleteUser0.json");
    twitter.push("twitter_small_records_deleteUserLang0.json");
    twitter.push("twitter_small_records_changeUserLang25.json");
    twitter.push("twitter_small_records_deleteUser25.json");
    twitter.push("twitter_small_records_deleteUserLang25.json");
    twitter.push("twitter_small_records_changeUserLang50.json");
    twitter.push("twitter_small_records_deleteUser50.json");
    twitter.push("twitter_small_records_deleteUserLang50.json");
    twitter.push("twitter_small_records_changeUserLang75.json");
    twitter.push("twitter_small_records_deleteUser75.json");
    twitter.push("twitter_small_records_deleteUserLang75.json");
    twitter.push("twitter_small_records_changeUserLang100.json");
    twitter.push("twitter_small_records_deleteUser100.json");
    twitter.push("twitter_small_records_deleteUserLang100.json");

    for (var dataset of twitter) {
        await execute(warmup_query, repeat_query, dataset, query_TT4, "TT4");
    }
}

main()