#include "ChunkRenderer.h"
#include <thread>
#include <future>

bool ChunkRenderer::Init(std::unordered_map<chunkID, ChunkData*>* chunks, std::mutex* mut)
{
	this->chunks = chunks;
	this->chunks_access = mut;

	blockAtlas.FromFile("assets/blockAtlas.png");
	

	chunkVAO.Init(
		{
			{ CreateAttribute<0, glm::ivec4, 0, 0>, chunkVBO }
		}
	);

	chunkProgram.Init(
		{
			{ GL_VERTEX_SHADER, "chunk.vert" },
			{ GL_FRAGMENT_SHADER, "chunk.frag" }
		},
		{
			{0, "vs_in_blockData" }
		}
	);

	chunkProgram.SetTexture("blockAtlas", 1, blockAtlas);
	glActiveTexture(GL_TEXTURE1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_NEAREST_MIPMAP_LINEAR);

	return true;
}

void ChunkRenderer::Clean()
{
	blockAtlas.Clean();
	chunkVBO.Clean();
	chunkProgram.Clean();
}

void ChunkRenderer::Render(ChunkData* chunk, glm::mat4& mvp)
{
	if (chunk->needsUpdate) {
		chunk->needsUpdate = false;
		std::thread th(&ChunkRenderer::BuildChunkMesh, this, chunk);
		th.detach();
	}

	
	chunkVBO.BufferData(chunk->vertices);
	chunkProgram.Use();
	chunkVAO.Bind();
	chunkProgram.SetTexture("blockAtlas", 1, blockAtlas);
	glTexParameteri(GL_TEXTURE_2D, GL_NEAREST_MIPMAP_NEAREST, 1);
	chunkProgram.SetUniform("MVP", mvp);
	glDrawArrays(GL_TRIANGLES, 0, chunk->vertices.size());

	chunkProgram.Unuse();
	

}

/// <summary> 
/// checks if a block is on the edge of the chunk 
/// </summary>
bool ChunkRenderer::isBoundaryBlock(int x, int y, int z) {
	return x == 0 || x == 15 || y == 0 || y == 255 || z == 0 || z == 15;
}

/// <summary>
/// checks if a block is transparent
/// </summary>
bool ChunkRenderer::isTransparent(block b)
{
	return transparentBlocks.find(b) != transparentBlocks.end();
}

bool ChunkRenderer::isOpaque(block b)
{
	return transparentBlocks.find(b) == transparentBlocks.end();
}

int ChunkRenderer::blockFace(block b, int face) {
	switch (b) {
	case 1: // grass
	case 4:
		switch (face) {
		case 2:
			return 1;
		case 3:
			return 3;
		default:
			return 4;
		}
		break;
	case 6:
	case 7:
		switch (face) {
		case 2:
		case 3:
			return 7;
		default:
			return 6;
		}
	default:
		return b;
	}
}

void ChunkRenderer::BuildChunkMesh(ChunkData* chunk) {
	std::vector<glm::ivec4> vert;
	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 256; ++y) {
			for (int z = 0; z < 16; ++z) {
				if (chunk->blocks[x][y][z] == 0) continue; // if block is air, skip side checking
				else {
					if ((x == 15 && chunks->find({ chunk->id.x + 1, chunk->id.z }) == chunks->end()) ||
						isTransparent(x == 15 ? (*chunks)[{chunk->id.x + 1, chunk->id.z}]->blocks[0][y][z] : chunk->blocks[x + 1][y][z])) { // x pos {0}
						int tex = blockFace(chunk->blocks[x][y][z], 0); // texture id in first two bytes
						tex <<= 8; // left shift for face id
						tex |= 0; // face id in 3rd byte
						tex <<= 8; // left shift for vertex id
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z + 1, tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z    , tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z + 1, tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z    , tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z    , tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z + 1, tex | 2 });
					}
					if ((x == 0 && chunks->find({ chunk->id.x - 1, chunk->id.z }) == chunks->end()) ||
						isTransparent(x == 0 ? (*chunks)[{chunk->id.x - 1, chunk->id.z}]->blocks[15][y][z] : chunk->blocks[x - 1][y][z])) { // x neg {1}
						int tex = blockFace(chunk->blocks[x][y][z], 1);
						tex <<= 8;
						tex |= 1;
						tex <<= 8;
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z    , tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z    , tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z + 1, tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z    , tex | 2 });
					}
					if (y == 255 || isTransparent(chunk->blocks[x][y + 1][z])) { // y pos {2}
						int tex = blockFace(chunk->blocks[x][y][z], 2);
						tex <<= 8;
						tex |= 2;
						tex <<= 8;
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z + 1, tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z    , tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z    , tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z    , tex | 2 });
					}
					if (y == 0 || isTransparent(chunk->blocks[x][y - 1][z])) { // y neg {3}
						int tex = blockFace(chunk->blocks[x][y][z], 3);
						tex <<= 8;
						tex |= 3;
						tex <<= 8;
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z + 1, tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z    , tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z    , tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z    , tex | 2 });
					}
					if ((z == 15 && chunks->find({ chunk->id.x, chunk->id.z + 1 }) == chunks->end()) ||
						isTransparent(z == 15 ? (*chunks)[{chunk->id.x, chunk->id.z + 1}]->blocks[x][y][0] : chunk->blocks[x][y][z + 1])) { // z pos {4}
						int tex = blockFace(chunk->blocks[x][y][z], 4);
						tex <<= 8;
						tex |= 4;
						tex <<= 8;
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z + 1, tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z + 1, tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z + 1, tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z + 1, tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z + 1, tex | 2 });
					}
					if ((z == 0 && chunks->find({ chunk->id.x, chunk->id.z - 1 }) == chunks->end()) ||
						isTransparent(z == 0 ? (*chunks)[{chunk->id.x, chunk->id.z - 1}]->blocks[x][y][15] : chunk->blocks[x][y][z - 1])) { // z neg {5}
						int tex = blockFace(chunk->blocks[x][y][z], 5);
						tex <<= 8;
						tex |= 5;
						tex <<= 8;
						vert.push_back({ chunk->id.x * 16 + x + 1, y    , chunk->id.z * 16 + z    , tex | 0 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z    , tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z    , tex | 2 });
						vert.push_back({ chunk->id.x * 16 + x    , y    , chunk->id.z * 16 + z    , tex | 1 });
						vert.push_back({ chunk->id.x * 16 + x    , y + 1, chunk->id.z * 16 + z    , tex | 3 });
						vert.push_back({ chunk->id.x * 16 + x + 1, y + 1, chunk->id.z * 16 + z    , tex | 2 });
					}
				}



			}
		}
	}
	chunk->vertices.clear();
	chunk->vertices = vert;
}




