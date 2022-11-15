#include "src/RecordLoader.h"
#include "src/BitmapIterator.h"
#include "src/BitmapConstructor.h"

#include <math.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

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

Record* getRecord(char* file) {
    char path[150];
    strcpy(path, base_dir);
    strcat(path, file);
    Record* rec = RecordLoader::loadSingleRecord(path);
    if (rec == NULL) {
        cout<<"record loading fails."<<endl;
        exit(-1);
    }
    return rec;
}

RecordSet* getRecordSet(char* file) {
    char path[150] = "/home/ubuntu/datasets/";
    strcat(path, file);
    RecordSet* rec_set = RecordLoader::loadRecords(path);
    if (rec_set->size() == 0) {
        cout<<"record loading fails."<<endl;
        exit(-1);
    }
    return rec_set;
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
vector<string> query_TT1(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 5;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            }
            iter->up();
        }
        delete iter;
        delete bm;
    }
    delete record_set;

    return result;
}

// $[*].user.lang
vector<string> query_TT1_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 5;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;

    return result;
}

// {$.user.lang, $.lang}
vector<string> query_TT2(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    unordered_set<char*> set;
    char* key = NULL;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        set.insert("lang");
        set.insert("user");
        while (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "lang") == 0) {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            } else {
                if (iter->down() == false) continue;  /* value of "user" */
                if (iter->isObject() && iter->moveToKey("lang")) {
                    // value of "lang"
                    char* value = iter->getValue();
                    result.push_back(value);
                    if (value) free(value);
                }
                iter->up();
            }
        }
        delete iter;
        delete bm;
    }
    delete record_set;
    
    return result;
}

// {$[*].user.lang, $[*].lang}
vector<string> query_TT2_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    unordered_set<char*> set;
    char* key = NULL;
    while (iter->isArray() && iter->moveNext() == true) {
        set.insert("lang");
        set.insert("user");
        if (iter->down() == false) continue;  /* array element on the top level */
        while (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "lang") == 0) {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            } else {
                if (iter->down() == false) continue;  /* value of "user" */
                if (iter->isObject() && iter->moveToKey("lang")) {
                    // value of "lang"
                    char* value = iter->getValue();
                    result.push_back(value);
                    if (value) free(value);
                }
                iter->up();
            }
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    
    return result;
}

// $.user.lang[?(@ == 'nl')]"
vector<string> query_TT3(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"nl\",") == 0) {
                    result.push_back(value);
                }
                if (value) free(value);
            }
            iter->up();
        }
        delete iter;
        delete bm;
    }
    delete record_set;
    
    return result;
}

// $[*].user.lang[?(@ == 'nl')]"
vector<string> query_TT3_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"nl\",") == 0) {
                    result.push_back(value);                  
                }
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    
    return result;
}

// $.user.lang[?(@ == 'en')]"
vector<string> query_TT4(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"en\",") == 0) {
                    result.push_back(value);
                }
                if (value) free(value);
            }
            iter->up();
        }
        delete iter;
        delete bm;
    }
    delete record_set;
    
    return result;
}

// $[*].user.lang[?(@ == 'en')]"
vector<string> query_TT4_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"en\",") == 0) {
                    result.push_back(value);
                }
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    
    return result;
}

// {$.bestMarketplacePrice.price, $.name}
vector<string> query_WM(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    unordered_set<char*> set;
    char* key = NULL;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        set.insert("bestMarketplacePrice");
        set.insert("name");
        while (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "bestMarketplacePrice") == 0) {
                if (iter->down() == false) continue;
                if (iter->isObject() && iter->moveToKey("price")) {
                    char* value = iter->getValue();
                    result.push_back(value);
                    if (value) free(value);
                }
                iter->up();
            } else {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            }
        }
        delete iter;
        delete bm;
    }
    delete record_set;

    return result;
}

// {$[*].bestMarketplacePrice.price, $[*].name}
vector<string> query_WM_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    unordered_set<char*> set;
    char* key = NULL;
    while (iter->isArray() && iter->moveNext() == true) {
        set.insert("bestMarketplacePrice");
        set.insert("name");
        if (iter->down() == false) continue;  /* array element on the top level */
        while (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "bestMarketplacePrice") == 0) {
                if (iter->down() == false) continue;
                if (iter->isObject() && iter->moveToKey("price")) {
                    char* value = iter->getValue();
                    result.push_back(value);
                    if (value) free(value);
                }
                iter->up();
            } else {
                char* value = iter->getValue();
                result.push_back(value);
                if (value) free(value);
            }
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;

    return result;
}

// $.categoryPath[1:3].id
vector<string> query_BB(char* file) {
    RecordSet* record_set = getRecordSet(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 3;

    /* process the records one by one: for each one, first build bitmap, then perform
     * the query with a bitmap iterator
     */
    int num_recs = record_set->size();
    Bitmap* bm = NULL;

    vector<string> result;
    for (int i = 0; i < num_recs; i++) {
        bm = BitmapConstructor::construct((*record_set)[i], thread_num, level_num);
        BitmapIterator* iter = BitmapConstructor::getIterator(bm);
        if (iter->isObject() && iter->moveToKey("categoryPath")) {
            if (iter->down() == false) continue;
            if (iter->isArray()) {
                for (int idx = 1; idx <= 2; ++idx) {
                    if (iter->moveToIndex(idx)) {
                        if (iter->down() == false) continue;
                        if (iter->isObject() && iter->moveToKey("id")) {
                            // value of "id"
                            char* value = iter->getValue();
                            result.push_back(value);
                            if (value) free(value);
                        }
                        iter->up();
                    }
                }
            }
            iter->up();
        }
        delete iter;
        delete bm;
    }
    delete record_set;

    return result;
}

// $[*].categoryPath[1:3].id
vector<string> query_BB_large(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 1;
    /* set the number of levels of bitmaps to create, either based on the
     * query or the JSON records. E.g., query $[*].user.id needs three levels
     * (level 0, 1, 2), but the record may be of more than three levels
     */
    int level_num = 4;

    /* process the input record: first build bitmap, then perform
     * the query with a bitmap iterator
     */
    Bitmap* bm = BitmapConstructor::construct(rec, thread_num, level_num);
    BitmapIterator* iter = BitmapConstructor::getIterator(bm);

    vector<string> result;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;  /* array element on the top level */
        if (iter->isObject() && iter->moveToKey("categoryPath")) {
            if (iter->down() == false) continue;
            if (iter->isArray()) {
                for (int idx = 1; idx <= 2; ++idx) {
                    if (iter->moveToIndex(idx)) {
                        if (iter->down() == false) continue;
                        if (iter->isObject() && iter->moveToKey("id")) {
                            // value of "id"
                            char* value = iter->getValue();
                            result.push_back(value);
                            if (value) free(value);
                        }
                        iter->up();
                    }
                }
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;

    return result;
}

void execute(char* dataset, vector<string> (*func)(char*), const char* query) {
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
    cout << "pison,"<<dataset<<","<<query<<","<<average<<","<<std<<","<<numResults<<","<<warmup_query<<","<<repeat_query << endl;
}