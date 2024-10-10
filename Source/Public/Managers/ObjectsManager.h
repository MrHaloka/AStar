//
// Created by MrHaloka.
//

#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H
#include "../Containers/SpatialHash2d.h"


class ObjectsManager {
public:
    ObjectsManager(uint8_t gridSize, uint16_t cellSize);

    void AddNewObject(uint32_t id, const Vector2d &position);
    void RemoveObject(uint32_t id, const Vector2d &position);
    std::vector<uint32_t> GetAllObjects(const Vector2d &position);
    uint32_t GetGridId(Vector2d position);

    std::vector<uint32_t> GetUnblockedNeighbours(const Vector2d& position);
    Vector2d GetGridFrom(const uint32_t gridId);

    uint16_t GetCellSize();

    bool IsBlocked(uint32_t gridId);

protected:
    SpatialHash2d mGridHash;
    std::unordered_map<uint32_t, Vector2d> mObjects;

};


#endif //OBJECTSMANAGER_H
