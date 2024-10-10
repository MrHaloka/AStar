//
// Created by MrHaloka
//

#ifndef GRIDINDEXER2D_H
#define GRIDINDEXER2D_H
#include <cstdint>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <optional>

#include "vector2d.h"

struct GridIndexer2d {
protected:
    const uint8_t mGridSize;
    const uint16_t mCellSize;
    const uint16_t mMaxID;

public:
    GridIndexer2d(uint8_t gridSize, uint16_t cellSize) : mGridSize(gridSize), mCellSize(cellSize), mMaxID(gridSize * gridSize)
    {
        assert(gridSize > 0);
        assert(cellSize > 0);
    }

    uint32_t ToGrid(const Vector2d& location) const
    {
       const int xIndex = std::clamp<int>(std::floor(location.x / mCellSize), 0, mGridSize-1);
       const int yIndex = std::clamp<int>(std::floor(location.y / mCellSize), 0, mGridSize-1);
       return xIndex + mGridSize * yIndex;
    }

    Vector2d FromGrid(uint32_t GridPoint) const
    {
        return Vector2d(GridPoint % mGridSize, static_cast<uint32_t>(std::floor(GridPoint / mGridSize))) * mCellSize;
    }

    //TODO:: fix negative values
    std::optional<uint32_t> GetNextColumn(uint32_t gridId, int8_t step = 1) const
    {
        int32_t nextColumn = static_cast<int32_t>(gridId) + step;
        if(nextColumn >= mMaxID || nextColumn < 0 || nextColumn / mGridSize != gridId / mGridSize){
            return std::nullopt;
        }
        return nextColumn;
    }
    
    //TODO:: fix negative values
     std::optional<uint32_t> GetNextRow(uint32_t gridId, int8_t Step = 1) const
    {
        int32_t nextRow = static_cast<int32_t>(gridId) + static_cast<int32_t>(mGridSize) * Step;
        if(nextRow >= mMaxID || nextRow < 0 || nextRow % mGridSize != gridId % mGridSize)
        {
            return std::nullopt;
        }
        return nextRow;
    }

    uint16_t GetCellSize() {
        return mCellSize;
    }

    uint8_t GetGridSize(){
        return mGridSize;
    }
};



#endif //GRIDINDEXER2D_H
