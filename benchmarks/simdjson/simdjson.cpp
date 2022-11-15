#include <iostream>
#include "src/simdjson.h"

#include <math.h>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

bool DEBUG = false;
int warmup_query = 5, repeat_query = 10;
char* base_dir;

void init(int argc, char *argv[], char *dir) {
    base_dir = dir;
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "DEBUG")) {
            DEBUG = true;
        }
        else if(!strncmp(argv[i], "warmup=", 7)) {
            warmup_query = atoi(&argv[i][7]);
        }
        else if(!strncmp(argv[i], "repeat=", 7)) {
            repeat_query = atoi(&argv[i][7]);
        }
    }
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double avg(uint64_t *delays, int n) {
    uint64_t sum = 0;
    for (int i=0; i<n; i++) {
        sum += delays[i];
    }
    return (double)sum/n;
}

double stdDev(uint64_t *delays, int n, int average) {
    int sum = 0;
    for (int i=0; i<n; i++) {
        sum += (delays[i]-average)*(delays[i]-average);
    }
    return sqrt((float)sum/(n-1));
}

// $.user.lang
vector<string_view> query_TT1(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            result.push_back(value);
        }
    }

    return result;
}

// {$.user.lang, $.lang}
vector<string_view> query_TT2(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            result.push_back(value);
        }

        elem = doc["lang"];
        error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            result.push_back(value);
        }
    }

    return result;
}

// $.user.lang[?(@ == 'nl')]"
vector<string_view> query_TT3(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            if ((string_view) value == "nl") {
                result.push_back(value);
            }
        }
    }

    return result;
}

// $.user.lang[?(@ == 'en')]"
vector<string_view> query_TT4(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["user"]["lang"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            if ((string_view) value == "en") {
                result.push_back(value);
            }
        }
    }

    return result;
}

// {$.bestMarketplacePrice.price, $.name}
vector<string_view> query_WM(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        auto elem = doc["bestMarketplacePrice"]["price"];
        simdjson::error_code error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            if (elem.is_double()) {
                double d = elem.get_double();
                string_view value = string_view(to_string(d));
                result.push_back(value);
            }  
        }

        elem = doc["name"];
        error = elem.error();
        if (error == simdjson::error_code::SUCCESS) {
            string_view value = elem.get_string();
            result.push_back(value);
        }
    }

    return result;
}

// $.categoryPath[1:3].id
vector<string_view> query_BB(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    simdjson::dom::parser parser;
    simdjson::dom::document_stream stream = parser.load_many(path);

    vector<string_view> result;
    for (simdjson::dom::element doc : stream) {
        int idx = 0;
        auto arr = doc["categoryPath"];
        for (auto value : arr) {
            if (idx >= 3) break;
            if (idx >= 1) {
                auto elem = value["id"];
                simdjson::error_code error = elem.error();
                if (error == simdjson::error_code::SUCCESS) {
                    string_view value = elem.get_string();
                    result.push_back(value);
                } 
            }
            idx++;
        }
    }

    return result;
}

void execute(const char* dataset, vector<string_view> (*func)(const char*), const char* query) {
    extern bool DEBUG;
    if (DEBUG) cout << "Starting warmup queries on dataset " << dataset << endl;
    vector<string_view> result;
    for (int i=0; i < warmup_query; i++) 
        result = func(dataset);
    int numResults = result.size();
    uint64_t delays[repeat_query];
    uint64_t begin_time;
    if (DEBUG) cout << "Starting query on dataset " << dataset << endl;
    for (int i=0; i < repeat_query; i++) {
        begin_time = timeSinceEpochMillisec();
        func(dataset);
        delays[i] = timeSinceEpochMillisec() - begin_time;
    }
    double average = avg(delays, repeat_query);
    double std = stdDev(delays, repeat_query, average);
    if (DEBUG) cout << "Executed query " << query << " on dataset " << dataset << " in " << average << "ms; results: " << numResults << endl;
    cout << "simdjson,"<<dataset<<","<<query<<","<<average<<","<<std<<","<<numResults<<","<<warmup_query<<","<<repeat_query << endl;
}