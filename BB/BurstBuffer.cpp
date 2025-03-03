#include "BurstBuffer.h"
#include <unordered_map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/time.h>
using std::string;
using std::list;
using std::unordered_map;
using std::cout;
using std::ifstream;
using std::ofstream;

class hashList {
    public:
        static unordered_map<string, float> fileSize;
        list<string> cache;
        unordered_map<string, list<string>::iterator> location;
        string prefixBB;
        double size;

    hashList() {}
    ~hashList() {}
    hashList(string prebb);
    void loadFromFile();
    void saveToFile();
    // void createHashFileToSize();
};

class LRUCache : public hashList{
    public:
        string prefixPFS;
        double capacity;
        int type;

        LRUCache() {}
        ~LRUCache();
        LRUCache(string prebb, string prepfs, double cap);

        void prefetch(string key);
        void LRUFlush();
        void read(const char* key);
        void wirte(const char* key);
        void output();
};

void initBB(LRUCache** BB);

void hashList::loadFromFile() {
    string filename = prefixBB + "/hashList";
    ifstream inFile(filename);
    if (!inFile) {
        //std::cerr << "can't open file " <<  filename << std::endl;
        return;
    }

    string line;
    while (std::getline(inFile, line)) {
        cache.emplace_back(line);
        location[line] = --cache.end();
        size += fileSize[line];
    }

    inFile.close();
}

void hashList::saveToFile() {
    string filename = prefixBB + "/hashList";
    ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "can't open file " << filename << '\n';
        return;
    }
    for (const auto& str : cache) {
        outFile << str << '\n';  // 将每个字符串写入文件，换行符分隔
    }
    outFile.close();
}

// void hashList::createHashFileToSize() {
//     this->fileSize["wrfinput"] = 1;
//     this->fileSize["wrfout0"] = 2;
//     this->fileSize["wrfout1"] = 2;
// }

hashList::hashList(string prebb): prefixBB(prebb), size(0) {
    // createHashFileToSize();
    loadFromFile();
}

LRUCache::LRUCache(string prebb, string prepfs, double cap) : hashList(prebb), prefixPFS(prepfs), capacity(cap){}

LRUCache::~LRUCache() {
    saveToFile();
}
void LRUCache::prefetch(string key) {
    if(location.count(key)) {
        cache.splice(cache.begin(), cache, location[key]);
    }
    else {
        while(size + fileSize[key] > capacity && !cache.empty()){
            LRUFlush();
        }
        cache.emplace_front(key);
        location[key] = cache.begin();
        size += fileSize[key];
        {
            struct timeval start;
            struct timeval end;
            gettimeofday(&start,NULL);
            string commandStr = "mv " + prefixPFS + "/" + key + " " + prefixBB;
            char* command = const_cast<char*>(commandStr.c_str()); 
            int status = system(command);
            gettimeofday(&end,NULL);
            if (status == 0) {
                float time_use= (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
                fprintf(stderr,"Prefetch file (%s)\n", key.c_str());
                fprintf(stderr,"Prefetch time_use is %.8f us\n", time_use);
            } else {
                fprintf(stderr,"Error Command: %s\n", command);
                exit(1);
            }
        }
    }
}

void LRUCache::LRUFlush() {
    auto it = --cache.end();
    size -= fileSize[*it];
    location.erase(*it);
    cache.erase(it);
    
    struct timeval start;
    struct timeval end;
    {
        gettimeofday(&start,NULL);
        string commandStr = "mv " + prefixBB + "/" + *it + " " + prefixPFS;
        char* command = const_cast<char*>(commandStr.c_str()); 
        int status = system(command);
        gettimeofday(&end,NULL);
        if (status == 0) {
            float time_use= (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
            fprintf(stderr,"Flush file (%s)\n", (*it).c_str());
            fprintf(stderr,"Flush time_use is %.8f us\n", time_use);
        } else {
            fprintf(stderr,"Error Command: %s\n",command);
            exit(1);
        }
    }
}

void LRUCache::read(const char* key) {
    if(location.count(key)) {
        cache.splice(cache.begin(), cache, location[key]);
    }
    else {
        prefetch(key);
    }
    fprintf(stderr,"Read from file (%s)\n", key);
}

void LRUCache::wirte(const char* key) {
    if(location.count(key)) {
        cache.splice(cache.begin(), cache, location[key]);
    }
    else {
        while(size + fileSize[key] > capacity && !cache.empty()){
            LRUFlush();
        }
        cache.emplace_front(key);
        location[key] = cache.begin();
        size += fileSize[key];
    }
    fprintf(stderr,"Written to file (%s)\n", key);
}

void LRUCache::output() {
    for(auto it = fileSize.begin(); it != fileSize.end(); ++it) {
        cout << it->first <<' ' << it->second << '\n';
    }
    for(auto it = cache.begin(); it != cache.end(); ++it) {
        cout << *it << '\n';
    }
    cout << size << '\n';
}

void initBB(LRUCache** BB) {
    const char* prebb = getenv("BB");
    const char* prepfs = getenv("PFS");
    const char* capacity = getenv("BBCapacity");
    const char* preCBB = getenv("CBB");
    if(preCBB)
    if(!prebb) {
        fprintf(stderr,"Error: Missing environment BB\n");
        exit(1); 
    }
    else if(!prepfs) {
        fprintf(stderr,"Error: Missing environment PFS\n");
        exit(1); 
    }
    else if(!capacity) {
        fprintf(stderr,"Error: Missing environment BBCapacity\n");
        exit(1); 
    }
    *BB = new LRUCache(prebb, prepfs, atof(capacity));

}

void readFile(const char* key) {
    // LRUCache* BB;
    // initBB(&BB);
    // BB->read(key);
    // delete BB;
}

void writeFile(const char* key) {
    // LRUCache* BB;
    // initBB(&BB);
    // BB->wirte(key);
    // delete BB;
}

unordered_map<string, float> hashList::fileSize = {
    {"wrfbdy_d01", 44913050},
    {"wrfinput_d01", 168528268},
    {"wrfout_d01_2019-11-26_12:00:00", 158228860},
    {"wrfout_d01_2019-11-26_13:00:00", 196518701},
    {"wrfout_d01_2019-11-26_14:00:00", 201618856},
    {"wrfout_d01_2019-11-26_15:00:00", 205125744},
    {"wrfout_d01_2019-11-26_16:00:00", 207231784},
    {"wrfout_d01_2019-11-26_17:00:00", 208410902},
    {"wrfout_d01_2019-11-26_18:00:00", 209390912},
    {"wrfout_d01_2019-11-26_19:00:00", 210381779},
    {"wrfout_d01_2019-11-26_20:00:00", 211179500},
    {"wrfout_d01_2019-11-26_21:00:00", 211631861},
    {"wrfout_d01_2019-11-26_22:00:00", 211539779},
    {"wrfout_d01_2019-11-26_23:00:00", 210812139},
    {"wrfout_d01_2019-11-27_00:00:00", 209943299},
    //{"wrfrst_d01_2019-11-26_12:00:00", 891757025},
    {"wrfrst_d01_2019-11-26_13:00:00", 891730286},
    {"wrfrst_d01_2019-11-26_14:00:00", 912662718},
    {"wrfrst_d01_2019-11-26_15:00:00", 929783294},
    {"wrfrst_d01_2019-11-26_16:00:00", 939527978},
    {"wrfrst_d01_2019-11-26_17:00:00", 944027501},
    {"wrfrst_d01_2019-11-26_18:00:00", 947385016},
    {"wrfrst_d01_2019-11-26_19:00:00", 950481032},
    {"wrfrst_d01_2019-11-26_20:00:00", 953112252},
    {"wrfrst_d01_2019-11-26_21:00:00", 953332603},
    {"wrfrst_d01_2019-11-26_22:00:00", 947905080},
    {"wrfrst_d01_2019-11-26_23:00:00", 936690277},
    {"wrfrst_d01_2019-11-27_00:00:00", 925969064}
};