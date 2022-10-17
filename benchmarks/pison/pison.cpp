#include "src/RecordLoader.h"
#include "src/BitmapIterator.h"
#include "src/BitmapConstructor.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

Record* getRecord(char* file) {
    char path[150] = "../../../datasets/";
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

// $[*].user.lang
int query_TT1(char* file) {
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

    string output = "";
    int count = 0;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                output.append(value).append(";");
                count++;
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;

    return count;
}

// {$[*].user.lang, $[*].lang}
int query_TT2(char* file) {
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

    string output = "";
    int count = 0;
    unordered_set<char*> set;
    char* key = NULL;
    while (iter->isArray() && iter->moveNext() == true) {
        set.insert("lang");
        set.insert("user");
        if (iter->down() == false) continue;  /* array element on the top level */

        if (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "lang") == 0) {
                char* value = iter->getValue();
                output.append(value).append(";");
                count++;
                if (value) free(value);
            } else {
                if (iter->down() == false) continue;  /* value of "user" */
                if (iter->isObject() && iter->moveToKey("lang")) {
                    // value of "lang"
                    char* value = iter->getValue();
                    output.append(value).append(";");
                    count++;
                    if (value) free(value);
                }
                iter->up();
            }
        } 
        if ((key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "lang") == 0) {
                char* value = iter->getValue();
                output.append(value).append(";");
                count++;
                if (value) free(value);
            } else {
                if (iter->down() == false) continue;  /* value of "user" */
                if (iter->isObject() && iter->moveToKey("lang")) {
                    // value of "lang"
                    char* value = iter->getValue();
                    output.append(value).append(";");
                    count++;
                    if (value) free(value);
                }
                iter->up();
            }
        }

        // if (iter->isObject() && iter->moveToKey("user") != NULL) {
        //     if (iter->down() == false) continue;  /* value of "user" */
        //     if (iter->isObject() && iter->moveToKey("lang")) {
        //         // value of "lang"
        //         char* value = iter->getValue();
        //         output.append(value).append(";");
        //         count++;
        //         cout << "2" << endl;
        //         if (value) free(value);
        //     }
        //     iter->up();
        // }
        // if (iter->isObject() && iter->moveToKey("lang") != NULL) {
        //     char* value = iter->getValue();
        //     output.append(value).append(";");
        //     count++;
        //     cout << "1" << endl;
        //     if (value) free(value);
        // }
        
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    
    return count;
}

// $[*].user.lang[?(@ == 'nl')]"
int query_TT3(char* file) {
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

    string output = "";
    int count = 0;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"nl\",") == 0) {
                    output.append(value).append(";");
                    count++;                    
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
    
    return count;
}

// $[*].user.lang[?(@ == 'en')]"
int query_TT4(char* file) {
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

    string output = "";
    int count = 0;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (strcmp(value, "\"en\",") == 0) {
                    output.append(value).append(";");
                    count++;
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
    
    return count;
}

// {$[*].bestMarketplacePrice.price, $[*].name}
int query_WM_large(char* file) {
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

    string output = "";
    int count = 0;
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
                    output.append(value).append(";");
                    count++;
                    if (value) free(value);
                }
                iter->up();
            } else {
                char* value = iter->getValue();
                output.append(value).append(";");
                count++;
                if (value) free(value);
            }
        }
        iter->up();
    }
        
    delete iter;
    delete bm;
    delete rec;

    return count;
}

// $.categoryPath[1:3].id
int query_BB_small(char* file) {
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

    string output = "";
    int count = 0;
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
                            output.append(value).append(";");
                            count++;
                            if (value) free(value);
                        }
                        iter->up();
                    }
                }
            }
            iter->up();
        }
        delete iter;
    }
    delete bm;
    delete record_set;

    return count;
}

// $[*].categoryPath[1:3].id
int query_BB_large(char* file) {
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

    string output = "";
    int count = 0;
    // iter->moveToKey("products");
    // iter->down();
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
                            output.append(value).append(";");
                            count++;
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

    return count;
}

void execute(const int warmup_query, const int repeat_query, char* dataset, int (*func)(char*), const char* query) {
    extern bool DEBUG;
    if (DEBUG) cout << "Starting warmup queries on dataset " << dataset << endl;
    int num_results;
    for (int i=0; i < warmup_query; i++) 
        num_results = func(dataset);
    uint64_t delay;
    uint64_t begin_time;
    if (DEBUG) cout << "Starting query on dataset " << dataset << endl;
    begin_time = timeSinceEpochMillisec();
    for (int i=0; i < repeat_query; i++) func(dataset);
    delay = timeSinceEpochMillisec() - begin_time;
    if (DEBUG) cout << "Executed query " << query << " on dataset " << dataset << " in " << delay/repeat_query << "ms; results: " << num_results << endl;
    cout << "pison,"<<dataset<<","<<query<<","<<delay/repeat_query<<","<<num_results<<","<<warmup_query<<","<<repeat_query << endl;
}

bool DEBUG = false;

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "DEBUG")) {
            DEBUG = true;
        }
    }
    const int warmup_query = 5, repeat_query = 10;

    char test [50] = "test_large_record.json";
    char twitter_large [50] = "twitter_large_record.json";
    char twitter_smaller [50] = "twitter_small_records_smaller.json";
    char bestbuy_small [50] = "bestbuy_small_records.json";
    char bestbuy_large [50] = "bestbuy_large_record.json";
    char walmart_large [50] = "walmart_large_record.json";

    execute(warmup_query, repeat_query, twitter_large, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_large, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_large, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_large, query_TT4, "TT4");  
    
    // execute(warmup_query, repeat_query, twitter_smaller, query_TT1, "TT1");
    // execute(warmup_query, repeat_query, twitter_smaller, query_TT2, "TT2");
    // execute(warmup_query, repeat_query, twitter_smaller, query_TT3, "TT3");
    // execute(warmup_query, repeat_query, twitter_smaller, query_TT4, "TT4");

    execute(warmup_query, repeat_query, walmart_large, query_WM_large, "WM_large");

    // execute(warmup_query, repeat_query, bestbuy_small, query_BB_small, "BB_small");
    execute(warmup_query, repeat_query, bestbuy_large, query_BB_large, "BB_large");

    return 0;
}

