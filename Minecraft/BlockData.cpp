#include "BlockData.h"

block BlockData::GetBlock(int x, int y, int z) {
	int cx = 0, cz = 0;
	if (x < 0 || x > 15) {
		cx = x / 16;
		x %= 16;
		if (x < 0) {
			x += 16;
			cx -= 1;
		}
	}
	if (z < 0 || z > 15) {
		cz = z / 16;
		z %= 16;
		if (z < 0) {
			z += 16;
			cz -= 1;
		}
	}
	std::lock_guard<std::mutex> guard(*chunks_access);
	if (chunks->find({ cx, cz }) == chunks->end()) return 0;
	return (*chunks)[{ cx, cz }]->blocks[x][y][z];
}

void BlockData::PlaceBlock(glm::ivec3 vec, block b) {
	int x = vec.x, y = vec.y, z = vec.z;
	int cx = 0, cz = 0;
	if (x < 0 || x > 15) {
		cx = x / 16;
		x %= 16;
		if (x < 0) {
			x += 16;
			cx -= 1;
		}
	}
	if (z < 0 || z > 15) {
		cz = z / 16;
		z %= 16;
		if (z < 0) {
			z += 16;
			cz -= 1;
		}
	}
	std::lock_guard<std::mutex> guard(*chunks_access);
	if (chunks->find({ cx, cz }) == chunks->end()) return;
	(*chunks)[{ cx, cz }]->blocks[x][y][z] = b;
	(*chunks)[{ cx, cz }]->needsUpdate = true;
}

void BlockData::DestroyBlock(glm::ivec3 vec) {
	int x = vec.x, y = vec.y, z = vec.z;
	int cx = 0, cz = 0;
	if (x < 0 || x > 15) {
		cx = x / 16;
		x %= 16;
		if (x < 0) {
			x += 16;
			cx -= 1;
		}
	}
	if (z < 0 || z > 15) {
		cz = z / 16;
		z %= 16;
		if (z < 0) {
			z += 16;
			cz -= 1;
		}
	}
	std::lock_guard<std::mutex> guard(*chunks_access);
	if (chunks->find({ cx, cz }) == chunks->end()) return;
	(*chunks)[{ cx, cz }]->blocks[x][y][z] = 0;
	(*chunks)[{ cx, cz }]->needsUpdate = true;
	if (x == 15) {
		auto& it = chunks->find({ cx + 1, cz });
		if (it != chunks->end()) it->second->needsUpdate = true;
	}
	if (x == 0) {
		auto& it = chunks->find({ cx - 1, cz });
		if (it != chunks->end()) it->second->needsUpdate = true;
	}
	if (z == 15) {
		auto& it = chunks->find({ cx, cz + 1 });
		if (it != chunks->end()) it->second->needsUpdate = true;
	}
	if (z == 0) {
		auto& it = chunks->find({ cx, cz - 1 });
		if (it != chunks->end()) it->second->needsUpdate = true;
	}
}