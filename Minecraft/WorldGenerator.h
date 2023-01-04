#pragma once

#include "BlockData.h"
#include <mutex>


class WorldGenerator
{
public:
	void Init(BlockData*, long, std::mutex*);
	void Clean();

	void GenerateChunks(std::vector<chunkID>);

	bool ChunkExists(chunkID id) { 
		return blockData->GetChunks()->find(id) != blockData->GetChunks()->end();
	}


protected:
	BlockData* blockData;
	long seed;
	std::mutex* chunks_access;
};
