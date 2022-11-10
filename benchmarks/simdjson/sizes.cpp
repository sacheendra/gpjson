#include "simdjson.cpp"

int main(int argc, char *argv[]) {
    char base_dir[] = "../../../datasets/";
    init(argc, argv, base_dir);

    char* twitter[7];
    twitter[0] = "twitter_small_records.json";
    twitter[1] = "twitter_small_records_0.125x.json";
    twitter[2] = "twitter_small_records_0.25x.json";
    twitter[3] = "twitter_small_records_0.5x.json";
    twitter[4] = "twitter_small_records_1.5x.json";
    twitter[5] = "twitter_small_records_2x.json";
    twitter[6] = "twitter_small_records_2.5x.json";

    for (int i=0; i<7; i++) {
        execute(twitter[i], query_TT1, "TT1");
        execute(twitter[i], query_TT2, "TT2");
        execute(twitter[i], query_TT3, "TT3");
        execute(twitter[i], query_TT4, "TT4"); 
    }
 
    return 0;
}