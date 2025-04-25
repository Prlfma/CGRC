#pragma once

#include "HalfEdge.h"
#include "Viewport.h"
#include "../../Interfaces/IRenderSystem.h"

struct bbox
{ 
glm::vec3 min;
glm::vec3 max;
};
class Mesh
{ 
public: 
Mesh(const HalfEdgeTable& halfEdgeTable);
void render(IRenderSystem& rs, glm::vec3& color);
const bbox& getBoundingBox() const; 
void applyTransformation(FaceHandle fh, const glm::mat4& trf);
void deleteFace(FaceHandle fh);
void moveMesh(const glm::vec3& offset);
const HalfEdgeTable& getHalfEdgeTable() const; 
FaceHandle intersectRay(const ray& r) const;
private: 
    HalfEdgeTable m_halfEdgeTable;
    bbox m_bbox;
};
