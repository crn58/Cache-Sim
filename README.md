# Cache-Sim
The code below creates a cache simulator. 


Cache_sim.cpp - A basic cache. 
The simulator will be invoked with three command-line arguments as follows: 
./cache_sim mem_size cache_size access_filename

 • mem_size - Size of the memory in blocks, which must be divisible by 2. 
 • cache_size - Size of the cache in blocks, must be a factor of mem_size.
 • access_filename - A limitless file with a list of block addresses separated 
 by a space. No address should exceed the mem_size

-----------------------------------------------------------------------------------------------------------------
# Input

Cache_simulator.cpp - A 2-level cache, that determines hits and miss percentages 

Example input in command prompt: 

./cache_simulator mem_size L1_cache_size L2_cache_size L1_associativity access_filename

Example 2
cache_simulator.exe 8192 512 8192 4 accesslist.txt

8192: Memory size in bytes.
512: L1 cache size in bytes.
8192: L2 cache size in bytes.
4: L1 cache associativity.
input_access.txt: File containing memory access addresses.


-------------------------------------------------------------------------------------------------------------------
# Output 
A file with the name of "hitmiss-out.txt" which has a line with the address and whether there was 
a hit or a miss. The program should also report on the hit/miss percentages. 
Example: 
Reference file "accesslist.txt" contains the line "1 3 4 1 3 1". 
Call the command: ./cache_sim 4 2 accesslist.txt.
 Resulting file "hitmiss-out.txt" should contain:

-------------------------------------------------------------------------------------------------------------------
# Building the code 
The simulator will take as input (i) the configuration parameters of a cache and (ii) a sequence of memory addresses requested by 
the CPU. It will then simulate the behavior of the configured cache on the given memory 
references. Upon completion, the simulator will report for each reference whether it is a hit or a 
miss in the cache. 

Each cache entry consists of a valid bit and a tag. The tag should be extracted from the 
memory address in the customary way. For this homework, no actual data will be 
stored in the cache. Hits and misses will be determined by analyzing the memory addresses. 



