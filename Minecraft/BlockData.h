#pragma once

#include "ChunkData.h"
#include <unordered_map>
#include <mutex>

class BlockData
{
public:
	std::unordered_map<block, std::string> blockNames = {
		{ 0, "air" },
		{ 1, "grass" },
		{ 2, "stone" },
		{ 3, "dirt" },
		{ 5, "wooden planks" },
		{ 6, "smooth stone" },
		{ 18, "bedrock" },
		{ 19, "sand" },
		{ 50, "glass" },
		{ 53, "leaves" },
	};

	void Init(std::unordered_map<chunkID, ChunkData*>* ch, std::mutex* mut) {
		this->chunks = ch;
		this->chunks_access = mut;
	}

	block GetBlock(int, int, int);
	void DestroyBlock(glm::ivec3 vec);
	void PlaceBlock(glm::ivec3, block b);
	
	std::unordered_map<chunkID, ChunkData*>* GetChunks() { return chunks; }

protected:
	std::unordered_map<chunkID, ChunkData*>* chunks;
	std::mutex* chunks_access;
};