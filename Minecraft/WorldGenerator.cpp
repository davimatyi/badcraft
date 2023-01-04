#include "WorldGenerator.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <future>

void WorldGenerator::Init(BlockData* blockData, long seed, std::mutex* mut)
{
	this->blockData = blockData;
	this->seed = seed;
	this->chunks_access = mut;
}

void WorldGenerator::Clean()
{
}


void GenerateAChunk(ChunkData* chunk, long _seed)
{
	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			chunk->blocks[x][0][z] = 18;
			for (int y = 1; y <= 28; y++)
				chunk->blocks[x][y][z] = 2;
			for (int y = 29; y <= 31; y++)
				chunk->blocks[x][y][z] = 3;
			chunk->blocks[x][32][z] = 1;
		}
	}
}


void ParallelGeneration(std::vector<chunkID> chunkList, std::unordered_map<chunkID, ChunkData*>& chunks, std::mutex& chunks_access, long _seed)
{
	for (auto& id : chunkList) {
		ChunkData* chunk = new ChunkData(id);

		GenerateAChunk(chunk, _seed);

		std::lock_guard<std::mutex> guard(chunks_access);
		chunks[id] = chunk;
	}

}


void WorldGenerator::GenerateChunks(std::vector<chunkID> chunkList)
{
	//std::vector<ChunkData*> chs;
	//std::vector<std::thread*> ths;

	auto* chunks = blockData->GetChunks();

	std::thread t(ParallelGeneration, chunkList, std::ref(*chunks), std::ref(*chunks_access), seed);
	t.detach();

	//std::packaged_task<void(std::vector<chunkID>, std::unordered_map<chunkID, ChunkData*>&, std::mutex&, long)> tsk(ParallelGeneration);
	//std::thread th(std::move(tsk), chunkList, std::ref(*chunks), std::ref(*chunks_access), seed);
	//th.detach();

	//for (chunkID id : chunkList) {
	//	if (ChunkExists(id)) {
	//		continue;
	//	}
	//	std::thread t(ParallelGeneration, id, std::ref(*chunks), seed);
	//	t.detach();

	//	/*ChunkData* c = new ChunkData(id);
	//	chs.push_back(c);
	//	std::thread* t = new std::thread(ParallelGeneration, std::ref(*c), seed);
	//	ths.push_back(t);*/
	//}

	//for (int i = 0; i < ths.size(); ++i) {
	//	if (ths[i]->joinable()) ths[i]->join();
	//	//(*(blockData->GetChunks()))[chs[i]->id] = chs[i];
	//}

}


