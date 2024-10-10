//
// Created by MrHaloka
//

#include "../../Public/Managers/ObjectsManager.h"

#include <array>

ObjectsManager::ObjectsManager(uint8_t gridSize, uint16_t cellSize) : mGridHash(gridSize, cellSize) {
}

void ObjectsManager::AddNewObject(uint32_t id, const Vector2d &position) {
    mGridHash.InsertPoint(id, position);
}

void ObjectsManager::RemoveObject(uint32_t id, const Vector2d &position) {
    assert(mGridHash.RemovePoint(id, position));
}

std::vector<uint32_t> ObjectsManager::GetAllObjects(const Vector2d &position) {
    return mGridHash.GetAllPoints(position);
}

uint32_t ObjectsManager::GetGridId(Vector2d position) {
    return mGridHash.GetGridId(position);
}

std::vector<uint32_t> ObjectsManager::GetUnblockedNeighbours(const Vector2d& position) {
    std::vector<uint32_t> neighbours;
    std::array<std::optional<uint32_t>, 4> neighboursId = mGridHash.GetNeighbours(position);

    for (int index = 0 ; index < 4; ++index)
    {
        if (!neighboursId[index].has_value() || IsBlocked(neighboursId[index].value())){
            continue;
        }
        neighbours.push_back(neighboursId[index].value());
    }
    return neighbours;
}

bool ObjectsManager::IsBlocked(uint32_t gridId) {
    return mGridHash.HasPoint(gridId);
}

Vector2d ObjectsManager::GetGridFrom(const uint32_t gridId) {
    return mGridHash.GetGridFrom(gridId);
}

uint16_t ObjectsManager::GetCellSize() {
    return mGridHash.GetCellSize();
}
