#include "SelectionHandler.h"

bool SelectionHandler::Init(gCamera* camera, BlockData* blockData)
{
	this->camera = camera;
	this->blockData = blockData;
	return true;
}

void SelectionHandler::Clean()
{
}

block SelectionHandler::GetSelectedBlock()
{
	glm::vec3 playerPos = camera->GetEye();
	glm::vec3 rayDir = glm::normalize(camera->GetAt() - camera->GetEye());

	float distance = 0;
	block currentBlock = 0;
	glm::vec3 point = playerPos;

	while (distance <= reach && currentBlock == 0) // marching ray away from camera until end of reach or hit a block
	{
		point += rayDir * increment;
		distance += increment;
		currentBlock = blockData->GetBlock(floor(point.x), floor(point.y), floor(point.z));
	}

	return currentBlock; // returns highlighted block id
}

glm::ivec3 SelectionHandler::GetBlockPos()
{
	glm::vec3 playerPos = camera->GetEye();
	glm::vec3 rayDir = glm::normalize(camera->GetAt() - camera->GetEye());

	float distance = 0;
	block currentBlock = 0;
	glm::vec3 point = playerPos;

	while (distance <= reach && currentBlock == 0)
	{
		point += rayDir * increment; 
		distance += increment;
		currentBlock = blockData->GetBlock(floor(point.x), floor(point.y), floor(point.z));
	}
	return glm::ivec3(floor(point.x), floor(point.y), floor(point.z)); // returns highlighted block's position
}

glm::ivec3 SelectionHandler::GetPlacePos()
{
	glm::vec3 playerPos = camera->GetEye();
	glm::vec3 rayDir = glm::normalize(camera->GetAt() - camera->GetEye());

	float distance = 0;
	block currentBlock = 0;
	glm::vec3 point = playerPos;

	while (distance <= reach && currentBlock == 0)
	{
		point += rayDir * increment;
		distance += increment;
		currentBlock = blockData->GetBlock(floor(point.x), floor(point.y), floor(point.z));
	}
	if (currentBlock != 0) point -= rayDir * increment; // step back to last air block

	return glm::ivec3(floor(point.x), floor(point.y), floor(point.z)); // returns last air block position before block hit
}

