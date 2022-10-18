#include "pison.cpp"

int main(int argc, char *argv[]) {
    char base_dir[] = "../../../datasets/";
    init(argc, argv, base_dir);

    char twitter_large[50] = "twitter_large_record.json";
    char bestbuy_large[50] = "bestbuy_large_record.json";
    char walmart_large[50] = "walmart_large_record.json";

    execute(twitter_large, query_TT1, "TT1");
    execute(twitter_large, query_TT2, "TT2");
    execute(twitter_large, query_TT3, "TT3");
    execute(twitter_large, query_TT4, "TT4");  

    execute(walmart_large, query_WM_large, "WM_large");

    execute(bestbuy_large, query_BB_large, "BB_large");

    return 0;
}