#include "rapidjson.cpp"

int main(int argc, char *argv[]) {
    char base_dir[] = "../../../datasets/";
    init(argc, argv, base_dir);

    char twitter_small[50] = "twitter_small_records.json";
    char walmart_small[50] = "walmart_small_records.json";
    char bestbuy_small[50] = "bestbuy_small_records.json";

    execute(twitter_small, query_TT1, "TT1");
    execute(twitter_small, query_TT2, "TT2");
    execute(twitter_small, query_TT3, "TT3");
    execute(twitter_small, query_TT4, "TT4");  

    execute(walmart_small, query_WM, "WM");

    execute(bestbuy_small, query_BB, "BB");

    return 0;
}