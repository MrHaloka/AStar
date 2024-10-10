//
// Created by MrHaloka
//

#ifndef SPATIALHASH2D_H
#define SPATIALHASH2D_H
#include <unordered_map>
#include <vector>

#include "../DataStructures/GridIndexer2d.h"

class SpatialHash2d {
protected:
    std::unordered_multimap<uint32_t, uint32_t> mHash;
    GridIndexer2d mIndexer;

public:
    SpatialHash2d(uint8_t gridSize, uint16_t cellSize);

    void InsertPoint(uint32_t pointID, const Vector2d &location);
    bool RemovePoint(uint32_t pointID, const Vector2d &location);
    bool RemovePoint(uint32_t indexId, uint32_t pointID);
    bool UpdatePoint(uint32_t pointID, const Vector2d &oldLocation, const Vector2d &currentLocation);
    std::vector<uint32_t> GetAllPoints(const Vector2d &position);

    std::array<std::optional<uint32_t>, 4> GetNeighbours(const Vector2d &position);

    uint32_t GetGridId(Vector2d position) const;
    Vector2d GetGridFrom(uint32_t gridId);

    bool HasPoint(uint32_t gridId);

    uint16_t GetCellSize();
};


#endif //SPATIALHASH2D_H
