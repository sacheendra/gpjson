#include "pison.cpp"

int main(int argc, char *argv[]) {
    char base_dir[] = "../../../datasets/";
    init(argc, argv, base_dir);

    char* twitter[15];
    twitter[0] = "twitter_small_records_changeUserLang0.json";
    twitter[1] = "twitter_small_records_deleteUser0.json";
    twitter[2] = "twitter_small_records_deleteUserLang0.json";
    twitter[3] = "twitter_small_records_changeUserLang25.json";
    twitter[4] = "twitter_small_records_deleteUser25.json";
    twitter[5] = "twitter_small_records_deleteUserLang25.json";
    twitter[6] = "twitter_small_records_changeUserLang50.json";
    twitter[7] = "twitter_small_records_deleteUser50.json";
    twitter[8] = "twitter_small_records_deleteUserLang50.json";
    twitter[9] = "twitter_small_records_changeUserLang75.json";
    twitter[10] = "twitter_small_records_deleteUser75.json";
    twitter[11] = "twitter_small_records_deleteUserLang75.json";
    twitter[12] = "twitter_small_records_changeUserLang100.json";
    twitter[13] = "twitter_small_records_deleteUser100.json";
    twitter[14] = "twitter_small_records_deleteUserLang100.json";

    for (int i=0; i<15; i++) {
        execute(twitter[i], query_TT1, "TT1");
        execute(twitter[i], query_TT2, "TT2");
        execute(twitter[i], query_TT3, "TT3");
        execute(twitter[i], query_TT4, "TT4"); 
    }
 
    return 0;
}