#include <iostream>
#include <iomanip>
#include <cmath>
#include <queue>
#include <time.h>
#include <stdlib.h>
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)

///////////////////////////////////
struct cache
{
	unsigned int tag;
	bool valid;
	int frequency;
	int time;
};
cache CacheSIM[CACHE_SIZE];
int fullAssocCount = 0;
int LRU_COUNT = 0;
int nAssocCount[64*1024];
int firsOutCount = 0;
int line_size = 32; //from powers of 2 to 7 (4, 8, 16, 32, 64, 128)
int lines = CACHE_SIZE / line_size;
//////////////////////////////////////

enum cacheResType { MISS = 0, HIT = 1 };
unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */

unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGen1()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return  rand_() % (128 * 1024);
}

unsigned int memGen3()
{
	return rand_() % (DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr = 0;
	return (addr++) % (1024);
}

unsigned int memGen5()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
	static unsigned int addr = 0;
	return (addr += 256) % (DRAM_SIZE);
}

//takes in address 32 bit that contains tag, index, and offset
//We want the tag and the index so this is what we will do to get it
cacheResType directMap_SIM(unsigned int addr) {
	addr /= line_size; 
	//int index_size = pow(2, log2(lines));
	unsigned int index = addr % lines; 
	addr /= lines; 
	unsigned int tag = addr;
	if (CacheSIM[index].tag == tag && CacheSIM[index].valid) { 
		return HIT;
	}
	else {
		//if not hit then we store the new tag in the cache with the index we got and change the validity to true
		CacheSIM[index].tag = tag;
		CacheSIM[index].valid = true;
		return MISS;
	}
}

cacheResType FullyAssoc_SIM_Random(unsigned int addr) {
	unsigned int tag = addr / line_size;
	for (int i = 0; i < fullAssocCount; i++) {
		if (CacheSIM[i].tag == tag && CacheSIM[i].valid) {
			return HIT;
		}
	}
	if (fullAssocCount >= CACHE_SIZE) {
		int loc = (rand() % CACHE_SIZE);
		CacheSIM[loc].tag = tag;
		CacheSIM[loc].valid = true;
	}
	else {
		CacheSIM[fullAssocCount].tag = tag;
		CacheSIM[fullAssocCount].valid = true;
		fullAssocCount++;
	}

	return MISS;

}

cacheResType FullyAssoc_SIM_LRU(unsigned int addr) {
	unsigned int tag = addr / line_size;
	LRU_COUNT++;
	for (int i = 0; i < fullAssocCount; i++) {
		if (CacheSIM[i].tag == tag && CacheSIM[i].valid) {
			CacheSIM[i].time = LRU_COUNT;
			return HIT;
		}
	}
	if (fullAssocCount == CACHE_SIZE) {
		int min = 1e10, loc = 0;
		for (int i = 0; i < CACHE_SIZE; i++) {
			if (CacheSIM[i].time < min) {
				min = CacheSIM[i].time;
				loc = i;
			}
		}
		loc = (rand() % CACHE_SIZE);
		CacheSIM[loc].tag = tag;
		CacheSIM[loc].valid = true;
		CacheSIM[loc].time = LRU_COUNT;
	}
	else {
		CacheSIM[fullAssocCount].tag = tag;
		CacheSIM[fullAssocCount].valid = true;
		CacheSIM[fullAssocCount].time = LRU_COUNT;
		fullAssocCount++;
	}

	return MISS;

}

cacheResType FullyAssoc_SIM_LFU(unsigned int addr) {
	unsigned int tag = addr / line_size;
	LRU_COUNT++;
	for (int i = 0; i < fullAssocCount; i++) {
		if (CacheSIM[i].tag == tag && CacheSIM[i].valid) {
			CacheSIM[i].time = LRU_COUNT;
			CacheSIM[i].frequency++;
			return HIT;
		}
	}
	if (fullAssocCount == CACHE_SIZE) {
		int min = 1e10, loc = 0;
		for (int i = 0; i < CACHE_SIZE; i++) {
			if (CacheSIM[i].frequency < min) {
				min = CacheSIM[i].frequency;
				loc = i;
			}
		}
		loc = (rand() % CACHE_SIZE);
		CacheSIM[loc].tag = tag;
		CacheSIM[loc].valid = true;
		CacheSIM[loc].frequency++;
	}
	else {
		CacheSIM[fullAssocCount].tag = tag;
		CacheSIM[fullAssocCount].valid = true;
		CacheSIM[fullAssocCount].frequency++;
		fullAssocCount++;
	}
	return MISS;
}

cacheResType FullyAssoc_SIM_FIFO(unsigned int addr) {
	unsigned int tag = addr / line_size;
	LRU_COUNT++;
	for (int i = 0; i < fullAssocCount; i++) {
		if (CacheSIM[i].tag == tag && CacheSIM[i].valid) {
			return HIT;
		}
	}
	if (fullAssocCount >= CACHE_SIZE) {
		if (firsOutCount >= CACHE_SIZE) firsOutCount = 0;
		CacheSIM[firsOutCount].tag = tag;
		CacheSIM[firsOutCount].valid = true;
		firsOutCount++;
	}
	else {
		CacheSIM[fullAssocCount].tag = tag;
		CacheSIM[fullAssocCount].valid = true;
		fullAssocCount++;
	}
	return MISS;
}

cacheResType nAssociative_SIM(unsigned int addr, int nWay) {
	int nSets = CACHE_SIZE / nWay;
	//int bits = log2(nSets);
	addr /= line_size; 
	unsigned int set = addr % nSets;
	unsigned int tag = addr / nSets;
	unsigned int index = set * nWay;
	for (int i = 0; i < nAssocCount[set]; i++) {
		if (CacheSIM[index].tag == tag && CacheSIM[index].valid) {
			return HIT;
		}
		index++;
	}
	if (nAssocCount[set] >= nWay) {
		int loc = (rand() % nWay) + index;
		CacheSIM[loc].tag = tag;
		CacheSIM[loc].valid = true;
	}
	else {
		CacheSIM[index + nAssocCount[set]].tag = tag;
		CacheSIM[index + nAssocCount[set]].valid = true;
		nAssocCount[set]++;
	}
	return MISS;
}

// Cache Simulator
/* 
cacheResType cacheSim(unsigned int addr)
{
		
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}
*/

char *msg[2] = { "Miss","Hit" };

void cacheClear() {
	for (int i = 0; i < CACHE_SIZE; i++) {
		CacheSIM[i].frequency = 0;
		CacheSIM[i].time = 0;
		CacheSIM[i].valid = false;
		CacheSIM[i].tag = 0;
	}
}

int main()
{
	for (int j = 0; j < CACHE_SIZE; j++) nAssocCount[j] = 0;
	srand(time(NULL));
	int inst = 0;
	cacheResType r;
	unsigned int addr;
	cout << "Cache Simulator\n";
	// change the number of iterations into 10,000,000 (10000000)

	int numWay = 16;
	cout << "Cache Size: " << CACHE_SIZE/1024 << "\t" << "Line size: " << line_size << " N-way: " << numWay << endl;
	int hit = 0, count = 0;

	cacheClear();
	for (; inst<10000000; inst++)
	{
		addr = memGen1();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_1:" << hit << endl;
	
	cacheClear();
	hit = 0, inst = 0;
	for (; inst<10000000; inst++)
	{
		addr = memGen2();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_2:" << hit << endl;
	
	cacheClear();
	hit = 0, inst = 0;
	for (; inst<10000000; inst++)
	{
		addr = memGen3();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_3:" << hit << endl;

	cacheClear();
	hit = 0, inst = 0;
	for (; inst<10000000; inst++)
	{
		addr = memGen4();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_4:" << hit << endl;

	cacheClear();
	hit = 0, inst = 0;
	for (; inst<10000000; inst++)
	{
		addr = memGen5();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_5:" << hit << endl;

	cacheClear();
	hit = 0, inst = 0;
	for (; inst<10000000; inst++)
	{
		addr = memGen6();
		r = nAssociative_SIM(addr, numWay);
		if (r == HIT) hit++;
		//count++;
		//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "MEM_6:" << hit << endl;

	//cout << count << endl;
	system("pause");
}
