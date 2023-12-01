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
#include<string>
#include <cmath>

using namespace std;

// Structure to represent cache entries
struct CacheEntry {
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
    MISS,
    HIT
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " mem_size cache_size access_filename" << endl;
        return 1;
    }

    int memSize = stoi(argv[1]);
    int cacheSize = stoi(argv[2]);
    string accessFilename = argv[3];

    if (memSize <= 0 || cacheSize <= 0 || memSize % 2 != 0 || memSize % cacheSize != 0) {
        cerr << "Invalid input parameters. Please ensure correct sizes." << endl;
        return 1;
    }

    // Initialize cache
    int numBlocksInMemory = memSize;
    int numBlocksInCache = cacheSize;
    vector<CacheEntry> cache(numBlocksInCache, { false, -1 });

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

    int hitCount = 0, missCount = 0;
    int address;
    while (accessFile >> address) {
        int tag = getTag(address, numBlocksInCache, numBlocksInMemory);

        bool hit = false;
        // Check cache
        for (int i = 0; i < numBlocksInCache; ++i) {
            if (cache[i].valid && cache[i].tag == tag) {
                hit = true;
                hitCount++;
                break;
            }
        }

        if (!hit) {
            missCount++;
            // Update cache with new entry
            int victimIndex = -1;
            for (int i = 0; i < numBlocksInCache; ++i) {
                if (!cache[i].valid) {
                    victimIndex = i;
                    break;
                }
            }

            if (victimIndex == -1) {
                // Cache is full, use a simple replacement policy (e.g., FIFO)
                victimIndex = 0;
            }

            // Replace/update the cache entry
            cache[victimIndex].valid = true;
            cache[victimIndex].tag = tag;

            output << address << " Miss" << endl;
        }
        else {
            output << address << " Hit" << endl;
        }
    }

    accessFile.close();
    output.close();

    int totalAccesses = hitCount + missCount;
    double hitRate = (static_cast<double>(hitCount) / totalAccesses) * 100.0;
    double missRate = (static_cast<double>(missCount) / totalAccesses) * 100.0;

    cout << "Out of " << totalAccesses << " memory accesses, there was a "
        << hitRate << "% cache hit rate and a " << missRate << "% miss rate." << endl;

    return 0;
}
