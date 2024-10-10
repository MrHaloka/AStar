//
// Created by MrHaloka.
//

#ifndef VERTEX_H
#define VERTEX_H
#include <cstdint>
#include <limits>

#include "vector2d.h"

struct Vertex {
    Vector2d gridLocation;
    double g;
    double h;
    uint32_t parent;
    uint32_t gridId;

    Vertex() : g(std::numeric_limits<double>::max()), h(std::numeric_limits<double>::max()), parent(std::numeric_limits<uint32_t>::max()), gridId(std::numeric_limits<uint32_t>::max())
    {
    }

    Vertex(const Vector2d& gridLocation, const double g, const double h, uint32_t parent, uint32_t gridId) :
    gridLocation(gridLocation), g(g), h(h), parent(parent), gridId(gridId)
    {
    }

    Vector2d GetLocation() const
    {
        return gridLocation;
    }

    double GetF() const
    {
        return g + h;
    }
};
#endif //VERTEX_H
