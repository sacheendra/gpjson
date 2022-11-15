#include <cstdio>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <math.h>

#include "src/document.h"
#include "src/filereadstream.h"

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
vector<string> query_TT1(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);
    
    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString()) {
                string value = d["user"]["lang"].GetString();
                result.push_back(value);
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

// {$.user.lang, $.lang}
vector<string> query_TT2(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString()) {
                string value = d["user"]["lang"].GetString();
                result.push_back(value);
            }
        }
        if (d.IsObject() && d.HasMember("lang") && d["lang"].IsString()) {
            string value = d["lang"].GetString();
            result.push_back(value);
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

// $.user.lang[?(@ == 'nl')]"
vector<string> query_TT3(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString() && d["user"]["lang"] == "nl") {
                string value = d["user"]["lang"].GetString();
                result.push_back(value);
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

// $.user.lang[?(@ == 'en')]"
vector<string> query_TT4(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("user") && d["user"].IsObject()) {
            if (d["user"].HasMember("lang") && d["user"]["lang"].IsString() && d["user"]["lang"] == "en") {
                string value = d["user"]["lang"].GetString();
                result.push_back(value);
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

// {$.bestMarketplacePrice.price, $.name}
vector<string> query_WM(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("bestMarketplacePrice") && d["bestMarketplacePrice"].IsObject()) {
            if (d["bestMarketplacePrice"].HasMember("price") && d["bestMarketplacePrice"]["price"].IsNumber()) {
                string value = to_string(d["bestMarketplacePrice"]["price"].GetDouble());
                result.push_back(value);
            }
        }
        if (d.IsObject() && d.HasMember("name") && d["name"].IsString()) {
            string value = d["name"].GetString();
            result.push_back(value);
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

// $.categoryPath[1:3].id
vector<string> query_BB(const char* dataset) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, dataset);

    FILE* fp = fopen(path, "r");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);

    vector<string> result;
    while(!d.HasParseError()) {
        if (d.IsObject() && d.HasMember("categoryPath") && d["categoryPath"].IsArray()) {
            for (int i=1; i<3 && i<d["categoryPath"].Size(); i++) {
                if (d["categoryPath"][i].IsObject() && d["categoryPath"][i].HasMember("id") && d["categoryPath"][i]["id"].IsString()) {
                    string value = d["categoryPath"][i]["id"].GetString();
                    result.push_back(value);
                }
            }
        }
        d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(is);
    }
    
    fclose(fp);
    return result;
}

void execute(const char* dataset, vector<string> (*func)(const char*), const char* query) {
    extern bool DEBUG;
    if (DEBUG) cout << "Starting warmup queries on dataset " << dataset << endl;
    vector<string> result;
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
    cout << "rapidjson,"<<dataset<<","<<query<<","<<average<<","<<std<<","<<numResults<<","<<warmup_query<<","<<repeat_query << endl;
}