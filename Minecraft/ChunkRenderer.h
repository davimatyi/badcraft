#pragma once

#include <set>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Includes/ProgramObject.h"
#include "Includes/VertexArrayObject.h"
#include "Includes/BufferObject.h"
#include "Includes/TextureObject.h"
#include "Includes/gCamera.h"

#include "ChunkData.h"
#include <mutex>

class ChunkRenderer
{
public:
	bool Init(std::unordered_map<chunkID, ChunkData*> *, std::mutex*);
	void Clean();

	void Render(ChunkData*, glm::mat4&);

	void BuildChunkMesh(ChunkData*);

protected:
	ProgramObject chunkProgram;
	VertexArrayObject chunkVAO;
	ArrayBuffer chunkVBO;

	Texture2D blockAtlas;

	std::unordered_map< chunkID, ChunkData* > *chunks;
	std::mutex* chunks_access;
	std::set<block> transparentBlocks = { 0, 50, 53 }; // air, leaves and glass for now

	bool isBoundaryBlock(int, int, int);
	bool isTransparent(block);
	bool isOpaque(block);
	int blockFace(block, int);

	

};