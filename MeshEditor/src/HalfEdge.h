#pragma once

#include "../../ThirdParty/glm/glm.hpp"
#include <vector>

#include <cstdint>


struct HalfEdgeHandle { int64_t index = -1; };
struct VertexHandle { int64_t index = -1; };
struct FaceHandle { int64_t index = -1; };
struct HalfEdge
{ 
//The face it belongs to, is invalid (== -1) if a boundary edge
FaceHandle fh;
//The vertex it points to is always valid
VertexHandle dst;
HalfEdgeHandle twin;
//The next HalfEdge in the CCW order is always valid
HalfEdgeHandle next;

HalfEdgeHandle prev; 
//HalfEdge is deleted if isDeleted equals true
 bool isDeleted;
};
struct Face
{ 
//One of the HalfEdges belonging to the Face, always valid
HalfEdgeHandle heh;
//Face is deleted if isDeleted equals true
 bool isDeleted;
};
struct Vertex
{ 
//An outgoing HalfEdge from this vertex. It is == -1 if the vertex is isolated
HalfEdgeHandle heh;
//Vertex is deleted if isDeleted equals true
 bool isDeleted;
};
class HalfEdgeTable
{ 
public: 
//Adds vertex
VertexHandle addVertex(glm::vec3 position); 
//Adds triangulated face
FaceHandle addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
//Adds quad face
FaceHandle addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2, 
VertexHandle vh3);
//Builds twins for half-edges. This function must be called in the end
void connectTwins();
//Marks face as deleted. Doesn’t remove it from array. 
void deleteFace(FaceHandle fh);
public: 
//For a given half-edge returns previous linked half-edge
HalfEdgeHandle prev(HalfEdgeHandle heh) const;
//For a given half-edge returns next linked half-edge
HalfEdgeHandle next(HalfEdgeHandle heh) const;
//For a given half-edge returns twin half-edge
HalfEdgeHandle twin(HalfEdgeHandle heh) const;
//For a given half-edge returns end vertex
VertexHandle destVertex(HalfEdgeHandle heh) const;
//For a given half-edge returns start vertex
VertexHandle sourceVertex(HalfEdgeHandle heh) const;
//For a given half-edge handle returns half-edge
HalfEdge& deref(HalfEdgeHandle vh);
const HalfEdge& deref(HalfEdgeHandle vh) const; 
//For a given half-edge returns half-edge handle
HalfEdgeHandle handle(const HalfEdge& v) const; 
Vertex& deref(VertexHandle vh);
const Vertex& deref(VertexHandle vh) const; 
VertexHandle handle(const Vertex& v) const; 
Face& deref(FaceHandle fh); 
const Face& deref(FaceHandle fh) const; 
FaceHandle handle(const Face& f) const; 
//For a given vertex handle set/get point
const glm::vec3& getPoint(VertexHandle handle) const; 
void setPoint(VertexHandle handle, glm::vec3 data);
//For a given half-edge handle set/get start point (vertex)
const glm::vec3& getStartPoint(HalfEdgeHandle handle) const; 
void setStartPoint(HalfEdgeHandle handle, glm::vec3 data);
//For a given half-edge handle set/get end point (vertex)
const glm::vec3& getEndPoint(HalfEdgeHandle handle) const; 
void setEndPoint(HalfEdgeHandle handle, glm::vec3 data);
const std::vector<Vertex>& getVertices() const; 
const std::vector<Face>& getFaces() const; 
private: 
std::vector<Vertex> vertices;
std::vector<HalfEdge> halfEdges;
std::vector<Face> faces;
std::vector<glm::vec3> points; 
};


