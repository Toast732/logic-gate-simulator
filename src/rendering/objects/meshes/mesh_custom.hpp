#pragma once

#include <vector>
#include <format>
#include <raylib-cpp.hpp>
#include <F:\Documents\GitHub\Applications\C++\Logic Gate Simulator\out\build\x64-Windows-Release\_deps\earcut.hpp-src\include\mapbox\earcut.hpp>
#include "../../../math/matrix_a.cpp"

struct Point {
	int x;
	int y;
};

typedef std::vector<Vector3> Vertices;
typedef std::vector<Point> Points;
typedef std::vector<std::vector<Point>> Polygon;

Polygon PolygonFromVertices(Vertices* vertices);

Mesh GenMeshVertices(Vertices vertices);