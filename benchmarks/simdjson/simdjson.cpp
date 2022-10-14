#include <iostream>
#include "src/simdjson.h"

#include <chrono>
#include <string>

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

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            // output.append(value).append(";");
            count++;
        }
    }

    return count;
}

// {$.user.lang, $.lang}
int query_TT2(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            // output.append(value).append(";");
            count++;
        }

        elem = doc["lang"];
        error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            // output.append(value).append(";");
            count++;
        }
    }

    return count;
}

// $.user.lang[?(@ == 'nl')]"
int query_TT3(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            if ((string_view) elem.get_string() == "nl") {
                // output.append(value).append(";");
                count++;
            }
        }
    }

    return count;
}

// $.user.lang[?(@ == 'en')]"
int query_TT4(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            if ((string_view) elem.get_string() == "en") {
                // output.append(value).append(";");
                count++;
            }
        }
    }

    return count;
}

// {$.bestMarketplacePrice.price, $.name}
int query_WM(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["bestMarketplacePrice"]["price"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            // output.append(value).append(";");
            count++;
        }

        elem = doc["name"];
        error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            // output.append(value).append(";");
            count++;
        }
    }

    return count;
}

// $.categoryPath[1:3].id
int query_BB(const char* dataset) {
    char path[150] = BASE;
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    string output = "";
    int count = 0;
    for (simdjson::dom::element doc : stream) {
        int idx = 0;
        auto arr = doc["categoryPath"];
        for (auto value : arr) {
            if (idx >= 3) break;
            if (idx >= 1) {
                auto elem = value["id"];
                simdjson::error_code error = elem.error();
                if (error == simdjson::error_code::SUCCESS) {
                    // output.append(value).append(";");
                    count++;
                } 
            }
            idx++;
        }
    }

    return count;
}

void execute(const int warmup_query, const int repeat_query, const char* dataset, int (*func)(const char*), const char* query) {
    cout << "Starting warmup queries on dataset " << dataset << endl;
    int num_results;
    for (int i=0; i < warmup_query; i++) 
        num_results = func(dataset);
    uint64_t delay;
    uint64_t begin_time;
    cout << "Starting query on dataset " << dataset << endl;
    begin_time = timeSinceEpochMillisec();
    for (int i=0; i < repeat_query; i++) func(dataset);
    delay = timeSinceEpochMillisec() - begin_time;
    cout << "Executed query " << query << " on dataset " << dataset << " in " << delay/repeat_query << "ms; results: " << num_results << endl;
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

    cout << "All queries done, exiting..." << endl;
    return 0;
}