#include <cstdio>
#include <iostream>
#include <chrono>
#include <string>

#include "src/document.h"
#include "src/filereadstream.h"

using namespace std;
#define BASE "../../../datasets/"

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// $.user.lang
int query_TT1(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);
    
    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString()) {
                count++;
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

// {$.user.lang, $.lang}
int query_TT2(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString()) {
                count++;
            }
        }
        if (d.IsObject() && d.HasMember("lang") && d["lang"].IsString()) {
            count++;
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

// $.user.lang[?(@ == 'nl')]"
int query_TT3(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString() && d["user"]["lang"] == "nl") {
                count++;
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

// $.user.lang[?(@ == 'en')]"
int query_TT4(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString() && d["user"]["lang"] == "en") {
                count++;
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

// {$.bestMarketplacePrice.price, $.name}
int query_WM(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("bestMarketplacePrice") && d["bestMarketplacePrice"].IsObject()) {
            if (d["bestMarketplacePrice"].HasMember("price") && d["bestMarketplacePrice"]["price"].IsString()) {
                count++;
            }
        }
        if (d.IsObject() && d.HasMember("name") && d["name"].IsString()) {
            count++;
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

// $.categoryPath[1:3].id
int query_BB(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    string output = "";
    int count = 0;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("categoryPath") && d["categoryPath"].IsArray()) {
            for (int i=1; i<3 && i<d["categoryPath"].Size(); i++) {
                if (d["categoryPath"][i].IsObject() && d["categoryPath"][i].HasMember("id") && d["categoryPath"][i]["id"].IsString()) {
                    count++;
                }
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return count;
}

void execute(const int warmup_query, const int repeat_query, const char* dataset, int (*func)(const char*), const char* query) {
    // cout << "Starting warmup queries on dataset " << dataset << endl;
    int num_results;
    for (int i=0; i < warmup_query; i++) 
        num_results = func(dataset);
    uint64_t delay;
    uint64_t begin_time;
    // cout << "Starting query on dataset " << dataset << endl;
    begin_time = timeSinceEpochMillisec();
    for (int i=0; i < repeat_query; i++) func(dataset);
    delay = timeSinceEpochMillisec() - begin_time;
    // cout << "Executed query " << query << " on dataset " << dataset << " in " << delay/repeat_query << "ms; results: " << num_results << endl;
    cout << "rapidjson,"<<dataset<<","<<query<<","<<delay/repeat_query<<","<<num_results<<","<<warmup_query<<","<<repeat_query << endl;
}

int main(void) {
    const int warmup_query = 5, repeat_query = 10;

    char test [50] = "test_small_records.json";
    char twitter_small [50] = "twitter_small_records.json";
    char twitter_smaller [50] = "twitter_small_records_smaller.json";
    char bestbuy_small [50] = "bestbuy_small_records.json";
    char walmart_small [50] = "walmart_small_records.json";

    execute(warmup_query, repeat_query, twitter_small, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_small, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_small, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_small, query_TT4, "TT4");

    execute(warmup_query, repeat_query, walmart_small, query_WM, "WM");

    execute(warmup_query, repeat_query, bestbuy_small, query_BB, "BB");

    return 0;
}