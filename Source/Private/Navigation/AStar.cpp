//
// Created by MrHaloka.
//

#include "../../Public/Navigation/AStar.h"
#include <cassert>
#include "../../Public/Managers/ObjectsManager.h"

AStar::AStar(ObjectsManager *ObjectsManager, Vector2d Start, Vector2d End) :
 mObjectsManager(ObjectsManager), mEndGridId(mObjectsManager->GetGridId(End)), mStartGridId(mObjectsManager->GetGridId(Start)), mEndGrid(End), mStartGrid(Start) {
    AddToVertexes(mStartGrid, 0);
    mOpenIDs.push(std::pair(std::numeric_limits<double>::max(), ObjectsManager->GetGridId(mStartGrid)));
}

PathLinkedList* AStar::CalculateThePath() {
    assert(mObjectsManager != nullptr);
    while (!mOpenIDs.empty()) {
        const uint32_t NewPoppedGridId = mOpenIDs.top().second;
        mOpenIDs.pop();
        auto currentVertex = mVertexes.find(NewPoppedGridId);
        assert(currentVertex != mVertexes.end());
        if (NewPoppedGridId == mEndGridId) {
            break;
        }

        mCloses.insert(currentVertex->second.gridId);
        for (const uint32_t NeighbourId: mObjectsManager->GetUnblockedNeighbours(currentVertex->second.GetLocation())) {
            if (mCloses.find(NeighbourId) != mCloses.end()) {
                continue;
            }
            if (mOpens.find(NeighbourId) == mOpens.end()) {
                AddToVertexes(mObjectsManager->GetGridFrom(NeighbourId));
            }
            assert(mVertexes.find(NeighbourId) != mVertexes.end());
            UpdateVertex(mVertexes.find(NeighbourId)->second, currentVertex->second);
        }
    }

    PathLinkedList *Path = LoadThePath();

    return Path;
}

void AStar::AddToVertexes(const Vector2d &vertexGridLocation, double g) {
    uint32_t gridId = mObjectsManager->GetGridId(vertexGridLocation);
    Vertex NewVertex = Vertex(
            vertexGridLocation,
            g,
            vertexGridLocation.Distance(mEndGrid),
            std::numeric_limits<uint32_t>::max(),
            gridId
    );
    mVertexes[gridId] = NewVertex;
}

void AStar::UpdateVertex(Vertex &vertex, Vertex &newParent) {
    double oldG = vertex.g;
    CalculateAndUpdateG(vertex, newParent);
    if (vertex.g < oldG) {
        mOpenIDs.push( std::pair(vertex.GetF(), vertex.gridId));
        mOpens.insert(vertex.gridId);
    }
}

void AStar::CalculateAndUpdateG(Vertex &vertex, const Vertex &newParent) {
    double newG = newParent.g + vertex.GetLocation().Distance(newParent.GetLocation());
    if (newG < vertex.g) {
        vertex.g = newG;
        vertex.parent = newParent.gridId;
    }
}

PathLinkedList *AStar::LoadThePath() {
    auto *path = new PathLinkedList(mEndGrid);
    if (mVertexes.find(mEndGridId) == mVertexes.end()){
        std::cout<< "No Path";
        return nullptr;
    }
    uint32_t lastVertexId = mVertexes.find(mEndGridId)->second.parent;
    while (lastVertexId != std::numeric_limits<uint32_t>::max() && mVertexes.find(lastVertexId)->second.parent != std::numeric_limits<uint32_t>::max()) {
        Vertex lastVertex = mVertexes.find(lastVertexId)->second;
        Vector2d location = lastVertex.GetLocation();
        auto *newPath = new PathLinkedList(location);
        path->AddAfter(newPath);
        path = newPath;
        lastVertexId = lastVertex.parent;
    }
    return path;
}