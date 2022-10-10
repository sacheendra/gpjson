#include "/home/ubuntu/Pison/src/RecordLoader.h"
#include "/home/ubuntu/Pison/src/BitmapIterator.h"
#include "/home/ubuntu/Pison/src/BitmapConstructor.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

Record* getRecord(char* file) {
    char path[150] = "/home/ubuntu/datasets/";
    strcat(path, file);
    Record* rec = RecordLoader::loadSingleRecord(path);
    if (rec == NULL) {
        cout<<"record loading fails."<<endl;
        exit(-1);
    }
    return rec;
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// $[*].user.lang
void query_TT1(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                output.append(value).append(";");
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    // cout << "matches are: " << output << endl;
}

// {$[*].user.lang, $[*].lang}
void query_TT2(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    unordered_set<char*> set;
    set.insert("user");
    set.insert("lang");
    char* key = NULL;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;  /* array element on the top level */
        if (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "lang") == 0) {
                char* value = iter->getValue();
                output.append(value).append(";");
                if (value) free(value);
            } else {
                if (iter->down() == false) continue;  /* value of "user" */
                if (iter->isObject() && iter->moveToKey("lang")) {
                    // value of "lang"
                    char* value = iter->getValue();
                    output.append(value).append(";");
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
    // cout << "matches are: " << output << endl;
}

// $[*].user.lang[?(@ == 'nl')]"
void query_TT3(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    string language = "nl";
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (language.compare(value) == 0)
                    output.append(value).append(";");
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    // cout<<"matches are: "<<output<<endl;
}

// $[*].user.lang[?(@ == 'en')]"
void query_TT4(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    string language = "en";
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
        if (iter->isObject() && iter->moveToKey("user")) {
            if (iter->down() == false) continue;
            if (iter->isObject() && iter->moveToKey("lang")) {
                char* value = iter->getValue();
                if (language.compare(value) == 0)
                    output.append(value).append(";");
                if (value) free(value);
            }
            iter->up();
        }
        iter->up();
    }
    delete iter;
    delete bm;
    delete rec;
    // cout<<"matches are: "<<output<<endl;
}

// {$[*].bestMarketplacePrice.price, $[*].name}
void query_WM(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    unordered_set<char*> set;
    set.insert("bestMarketplacePrice");
    set.insert("name");
    char* key = NULL;
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;  /* array element on the top level */
        while (iter->isObject() && (key = iter->moveToKey(set)) != NULL) {
            if (strcmp(key, "bestMarketplacePrice") == 0) {
                if (iter->down() == false) continue;
                if (iter->isObject() && iter->moveToKey("price")) {
                    char* value = iter->getValue();
                    output.append(value).append(";");
                    if (value) free(value);
                }
                iter->up();
            } else {
                char* value = iter->getValue();
                output.append(value).append(";");
                if (value) free(value);
            }
        }
        iter->up();
    }
        
    delete iter;
    delete bm;
    delete rec;
    // cout<<"matches are: "<<output<<endl;
}

// $[*].categoryPath[1:3].id
void query_BB(char* file) {
    Record* rec = getRecord(file);

    // set the number of threads for parallel bitmap construction
    int thread_num = 12;
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
    while (iter->isArray() && iter->moveNext() == true) {
        if (iter->down() == false) continue;
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
    //cout << "matches are: " << output << endl;
}

void execute(const int warmup_query, const int repeat_query, char* dataset, void (*func)(char*), const char* query) {
    cout << "Starting warmup queries on dataset " << dataset << endl;
    for (int i=0; i < warmup_query; i++) 
        func(dataset);
    uint64_t delay;
    uint64_t begin_time;
    cout << "Starting query on dataset " << dataset << endl;
    begin_time = timeSinceEpochMillisec();
    for (int i=0; i < repeat_query; i++) func(dataset);
    delay = timeSinceEpochMillisec() - begin_time;
    cout << "Executed query " << query << " on dataset " << dataset << " in " << delay/repeat_query << "ms" << endl;
}

int main() {
    const int warmup_query = 5, repeat_query = 10;

    char twitter_small [50] = "twitter_small_records.json";
    char twitter_smaller [50] = "twitter_small_records_smaller.json";
    char bestbuy_small [50] = "bestbuy_small_records.json";
    char walmart_small [50] = "walmart_small_records.json";

    execute(warmup_query, repeat_query, twitter_small, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_small, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_small, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_small, query_TT4, "TT4");  
    
    execute(warmup_query, repeat_query, twitter_smaller, query_TT1, "TT1");
    execute(warmup_query, repeat_query, twitter_smaller, query_TT2, "TT2");
    execute(warmup_query, repeat_query, twitter_smaller, query_TT3, "TT3");
    execute(warmup_query, repeat_query, twitter_smaller, query_TT4, "TT4");

    execute(warmup_query, repeat_query, bestbuy_small, query_BB, "BB");

    execute(warmup_query, repeat_query, walmart_small, query_WM, "WM");

    cout << "All queries done, exiting..." << endl;
    return 0;
}

