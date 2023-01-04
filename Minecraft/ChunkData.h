#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

typedef unsigned __int8 block;

struct chunkID
{
	int x;
	int z;
	chunkID(int x, int z) : x(x), z(z) {}
	friend bool operator==(const chunkID& l, const chunkID& r) {
		return l.x == r.x && l.z == r.z;
	}
	
};

//bool operator==(const chunkID& l, const chunkID& r) {
//	return l.x == r.x && l.z == r.z;
//}

namespace std
{
	template <>
	struct hash<chunkID>
	{
		std::size_t operator()(const chunkID& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			std::size_t h1 = std::hash<int>{}(k.x);
			std::size_t h2 = std::hash<int>{}(k.z);
			return h1 ^ (h2 << h1);
		}
	};
}


class ChunkData
{
public:
	block blocks[16][256][16] = { 0 };
	chunkID id;
	bool needsUpdate = true;
	std::vector<glm::ivec4> vertices;

	ChunkData() : id(0, 0) {}
	ChunkData(chunkID id) : id(id) {}
	ChunkData(ChunkData* p) : id(0,0) {}

};


#endif