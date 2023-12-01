//Author(s) : Kazi Ahsan, Shawn Spicer, Michaelangelo Bullard
// Due Date : 2023-11-30
// Extra Credit-3: Cache Simulator
//Instructor: Randall Klepetko

/*

The simulator will take as input (i) the configuration parameters of a cache and (ii) a sequence of memory addresses requested by
the CPU. It will then simulate the behavior of the configured cache on the given memory
references. Upon completion, the simulator will report for each reference whether it is a hit or a
miss in cache.
Each cache entry consists of a valid bit and a tag. The tag should be extracted from the
memory address in the customary way. For the purposes of this homework, no actual data will be
stored in the cache. Hits and misses will be determined by analyzing the memory addresses.


*/


#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include<string>

using namespace std;

// Structure to represent cache entries
struct CacheEntry
 {
    bool valid;
    int tag;
};

// Function to extract tag from memory address
int getTag(int address, int numBlocksInCache, int numBlocksInMemory) {
    int blockSize = numBlocksInMemory / numBlocksInCache;
    int tagBits = log2(blockSize);
    return address >> tagBits;
}

// Enum to classify cache miss types
enum class MissType {
    COMPULSORY,
    CAPACITY,
    CONFLICT
};

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " mem_size L1_cache_size L2_cache_size L1_associativity access_filename" << endl;
        return 1;
    }

    int memSize = stoi(argv[1]);
    int L1CacheSize = stoi(argv[2]);
    int L2CacheSize = stoi(argv[3]);
    int L1Associativity = stoi(argv[4]);
    string accessFilename = argv[5];

    if (memSize <= 0 || L1CacheSize <= 0 || L2CacheSize <= 0 || L1Associativity <= 0 ||
        memSize % 2 != 0 || memSize % (L1CacheSize / L1Associativity) != 0 ||
        L2CacheSize % L1CacheSize != 0 || L2CacheSize % L1Associativity != 0) {
        cerr << "Invalid input parameters. Please ensure correct sizes and associativity." << endl;
        return 1;
    }

    // Initialize L1 and L2 caches
    int numBlocksInMemory = memSize;
    int numBlocksInL1Cache = L1CacheSize / L1Associativity;
    int numBlocksInL2Cache = L2CacheSize;
    vector<vector<CacheEntry>> L1Cache(numBlocksInL1Cache / L1Associativity, vector<CacheEntry>(L1Associativity, {false, -1}));
    vector<CacheEntry> L2Cache(numBlocksInL2Cache, {false, -1});

    // Read memory access file
    ifstream accessFile(accessFilename);
    if (!accessFile.is_open()) {
        cerr << "Unable to open access file: " << accessFilename << endl;
        return 1;
    }

    ofstream output("hitmiss-out.txt");
    if (!output.is_open()) {
        cerr << "Unable to create output file." << endl;
        return 1;
    }

    int L1HitCount = 0, L2HitCount = 0;
    int L1MissCount = 0, L2MissCount = 0;
    int address;
    while (accessFile >> address) {
        int L1Tag = getTag(address, numBlocksInL1Cache, numBlocksInMemory);
        int L1SetIndex = L1Tag % (numBlocksInL1Cache / L1Associativity);

        bool L1Hit = false;
        // Check L1 cache
        for (int i = 0; i < L1Associativity; ++i) {
            if (L1Cache[L1SetIndex][i].valid && L1Cache[L1SetIndex][i].tag == L1Tag) {
                L1Hit = true;
                L1HitCount++;
                break;
            }
        }

        if (!L1Hit) {
            L1MissCount++;
            int L2Tag = getTag(address, numBlocksInL2Cache, numBlocksInMemory);
            int L2SetIndex = L2Tag % numBlocksInL2Cache;

            bool L2Hit = false;
            // Check L2 cache
            for (int i = 0; i < L1Associativity; ++i) {
                if (L2Cache[L2SetIndex * L1Associativity + i].valid &&
                    L2Cache[L2SetIndex * L1Associativity + i].tag == L2Tag) {
                    L2Hit = true;
                    L2HitCount++;
                    break;
                }
            }

            if (!L2Hit) {
                L2MissCount++;
                // Update L1 and L2 caches with new entry
                int L1VictimIndex = L1SetIndex * L1Associativity;
                if (L1Cache[L1SetIndex][L1Associativity - 1].valid) {
                    int replacedL1Tag = L1Cache[L1SetIndex][L1Associativity - 1].tag;
                    int replacedL2Tag = getTag(replacedL1Tag * L1Associativity, numBlocksInL2Cache, numBlocksInMemory);
                    int replacedL2SetIndex = replacedL2Tag % numBlocksInL2Cache;

                    bool conflict = false;
                    for (int i = 0; i < L1Associativity; ++i) {
                        if (L2Cache[replacedL2SetIndex * L1Associativity + i].valid &&
                            L2Cache[replacedL2SetIndex * L1Associativity + i].tag == replacedL2Tag) {
                            conflict = true;
                            break;
                        }
                    }

                    if (conflict) {
                        output << address << " Conflict Miss" << endl;
                    } else {
                        output << address << " Capacity Miss" << endl;
                    }
                } else {
                    output << address << " Compulsory Miss" << endl;
                }

                // Replace the victim entry in L1
                L1Cache[L1SetIndex][L1Associativity - 1].valid = true;
                L1Cache[L1SetIndex][L1Associativity - 1].tag = L1Tag;

                // Replace/update the corresponding entry in L2
                L2Cache[L2SetIndex * L1Associativity] = {true, L2Tag};
            } else {
                L1Cache[L1SetIndex][L1Associativity - 1].valid = true;
                L1Cache[L1SetIndex][L1Associativity - 1].tag = L1Tag;

                output << address << " Miss" << endl;
            }
        } else {
            output << address << " Hit" << endl;
        }
    }

    accessFile.close();
    output.close();

    int totalL1Accesses = L1HitCount + L1MissCount;
    double L1HitRate = (static_cast<double>(L1HitCount) / totalL1Accesses) * 100.0;
    double L1MissRate = (static_cast<double>(L1MissCount) / totalL1Accesses) * 100.0;

    int totalL2Accesses = L2HitCount + L2MissCount;
    double L2HitRate = (static_cast<double>(L2HitCount) / totalL2Accesses) * 100.0;
    double L2MissRate = (static_cast<double>(L2MissCount) / totalL2Accesses) * 100.0;

    cout << "L1 Cache: Out of " << totalL1Accesses << " memory accesses, there was a "
         << L1HitRate << "% hit rate and a " << L1MissRate << "% miss rate." << endl;

    cout << "L2 Cache: Out of " << totalL2Accesses << " memory accesses, there was a "
         << L2HitRate << "% hit rate and a " << L2MissRate << "% miss rate." << endl;

    return 0;
}
