#include "rapidjson.cpp"

int main(int argc, char *argv[]) {
    char base_dir[] = "../../../datasets/";
    init(argc, argv, base_dir);

    char* twitter[15];
    twitter[0] = "twitter_small_records_0changeUserLang.json";
    twitter[1] = "twitter_small_records_0deleteUser.json";
    twitter[2] = "twitter_small_records_0deleteUserLang.json";
    twitter[3] = "twitter_small_records_25changeUserLang.json";
    twitter[4] = "twitter_small_records_25deleteUser.json";
    twitter[5] = "twitter_small_records_25deleteUserLang.json";
    twitter[6] = "twitter_small_records_50changeUserLang.json";
    twitter[7] = "twitter_small_records_50deleteUser.json";
    twitter[8] = "twitter_small_records_50deleteUserLang.json";
    twitter[9] = "twitter_small_records_75changeUserLang.json";
    twitter[10] = "twitter_small_records_75deleteUser.json";
    twitter[11] = "twitter_small_records_75deleteUserLang.json";
    twitter[12] = "twitter_small_records_100changeUserLang.json";
    twitter[13] = "twitter_small_records_100deleteUser.json";
    twitter[14] = "twitter_small_records_100deleteUserLang.json";

    for (int i=0; i<15; i++) {
        execute(twitter[i], query_TT1, "TT1");
        execute(twitter[i], query_TT2, "TT2");
        execute(twitter[i], query_TT3, "TT3");
        execute(twitter[i], query_TT4, "TT4"); 
    }
 
    return 0;
}