//
// Created by MrHaloka
//

#include "../../Public/Containers/SpatialHash2d.h"

#include <array>

SpatialHash2d::SpatialHash2d(uint8_t gridSize, uint16_t cellSize) : mIndexer(GridIndexer2d(gridSize, cellSize))
{
}

/**
 * Insert at given position.
 * @param pointID the point's id to insert
 * @param location the location associated with this PointID
 */
void SpatialHash2d::InsertPoint(uint32_t pointID, const Vector2d& location)
{
    uint32_t indexID = mIndexer.ToGrid(location);
	mHash.insert({indexID, pointID});
}

/**
 * Remove at given position.
 * @param pointID the point's id to insert
 * @param location the location associated with this PointID
 * @return true if the value existed at this position
 */
bool SpatialHash2d::RemovePoint(uint32_t pointID, const Vector2d& location)
{
	uint32_t indexID = mIndexer.ToGrid(location);
    return RemovePoint(indexID, pointID);
}

/**
 * Remove at given position.
 * @param pointID the point's id to insert
 * @param indexId the index of this PointID
 * @return true if the value existed at this position
 */
bool SpatialHash2d::RemovePoint(uint32_t indexId, uint32_t pointID) {
	auto range = mHash.equal_range(indexId);
	for (auto it = range.first; it != range.second; ++it) {
		if (it->second == pointID) {
			mHash.erase(it);
			return true;
		}
	}
	return false;
}

/**
 * Move value from old to new position.
 * @param pointID the point's id to insert
 * @param oldLocation the old location associated with this point's id
 * @param currentLocation the current location for this point
 */
bool SpatialHash2d::UpdatePoint(uint32_t pointID, const Vector2d& oldLocation, const Vector2d& currentLocation)
{
	uint32_t oldIndexID = mIndexer.ToGrid(oldLocation);
	uint32_t currentIndexID = mIndexer.ToGrid(currentLocation);
	if (oldIndexID == currentIndexID)
	{
		return false;
	}
	bool bWasAtOldGrid = RemovePoint(oldIndexID, pointID);
	assert(bWasAtOldGrid);
	mHash.insert({currentIndexID, pointID});
	return true;
}

std::vector<uint32_t> SpatialHash2d::GetAllPoints(const Vector2d &position) {
	uint32_t indexId = mIndexer.ToGrid(position);
	auto range = mHash.equal_range(indexId);
	std::vector<uint32_t> results;
	for (auto it = range.first; it != range.second; ++it) {
		results.push_back(it->second);
	}
	return results;
}

uint32_t SpatialHash2d::GetGridId(Vector2d position) const {
	return mIndexer.ToGrid(position);
}

std::array<std::optional<uint32_t>, 4> SpatialHash2d::GetNeighbours(const Vector2d &position) {
    uint32_t gridID = mIndexer.ToGrid(position);
    std::array<std::optional<uint32_t>, 4> neighboursId;

    neighboursId[0] = mIndexer.GetNextRow(gridID, 1);
    neighboursId[1] = mIndexer.GetNextColumn(gridID, 1);
    neighboursId[2] = mIndexer.GetNextRow(gridID, -1);
    neighboursId[3] =  mIndexer.GetNextColumn(gridID, -1);

    return neighboursId;
}

Vector2d SpatialHash2d::GetGridFrom(uint32_t gridId) {
    return mIndexer.FromGrid(gridId);
}

bool SpatialHash2d::HasPoint(uint32_t gridId) {
    return mHash.find(gridId) != mHash.end();
}

uint16_t SpatialHash2d::GetCellSize() {
    return mIndexer.GetCellSize();
}
