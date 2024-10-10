//
// Created by MrHaloka.
//

#ifndef ASTAR_H
#define ASTAR_H
#include <cstdint>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "../DataStructures/vector2d.h"
#include "../DataStructures/PathLinkedList.h"
#include "../DataStructures/Vertex.h"

class ObjectsManager;

typedef std::pair<double, uint32_t> DataType;
class AStar {
public:
    AStar(ObjectsManager* ObjectsManager, Vector2d Start, Vector2d End);
    PathLinkedList* CalculateThePath();
protected:
    ObjectsManager* mObjectsManager;
    uint32_t mEndGridId;
    uint32_t mStartGridId;
    Vector2d mEndGrid;
    Vector2d mStartGrid;
    std::priority_queue<DataType, std::vector<DataType>, std::greater<>> mOpenIDs;
    std::unordered_map<uint32_t, Vertex> mVertexes;
    std::unordered_set<uint32_t> mOpens;
    std::unordered_set<uint32_t> mCloses;
protected:
    void AddToVertexes(const Vector2d &vertexGridLocation, double g = std::numeric_limits<uint32_t>::max());
    void UpdateVertex(Vertex &vertex, Vertex &newParent);
    void CalculateAndUpdateG(Vertex &vertex, const Vertex &newParent);

    PathLinkedList *LoadThePath();
};



#endif //ASTAR_H
