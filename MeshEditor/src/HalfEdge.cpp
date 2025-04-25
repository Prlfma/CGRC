#include "HalfEdge.h"
#include <unordered_map>


VertexHandle HalfEdgeTable::addVertex(glm::vec3 position) {
    Vertex v;
    v.isDeleted = false;
    v.heh = HalfEdgeHandle{-1}; // Ізольована вершина спочатку
    vertices.push_back(v);
    points.push_back(position);
    return VertexHandle{static_cast<int64_t>(vertices.size() - 1)};
}

FaceHandle HalfEdgeTable::addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) {
    Face f;
    f.isDeleted = false;

    HalfEdgeHandle heh0{static_cast<int64_t>(halfEdges.size())};
    HalfEdgeHandle heh1{heh0.index + 1};
    HalfEdgeHandle heh2{heh0.index + 2};

    HalfEdge e0, e1, e2;
    e0.dst = vh1;
    e1.dst = vh2;
    e2.dst = vh0;

    e0.next = heh1;
    e1.next = heh2;
    e2.next = heh0;

    e0.prev = heh2;
    e1.prev = heh0;
    e2.prev = heh1;

    e0.fh = FaceHandle{static_cast<int64_t>(faces.size())};
    e1.fh = e0.fh;
    e2.fh = e0.fh;

    e0.isDeleted = false;
    e1.isDeleted = false;
    e2.isDeleted = false;

    // Twins будуть проставлені пізніше
    e0.twin = HalfEdgeHandle{-1};
    e1.twin = HalfEdgeHandle{-1};
    e2.twin = HalfEdgeHandle{-1};

    halfEdges.push_back(e0);
    halfEdges.push_back(e1);
    halfEdges.push_back(e2);

    f.heh = heh0;
    faces.push_back(f);

    // Прописуємо будь-який вихідний ребро для вершини
    if (vertices[vh0.index].heh.index == -1) vertices[vh0.index].heh = heh2;
    if (vertices[vh1.index].heh.index == -1) vertices[vh1.index].heh = heh0;
    if (vertices[vh2.index].heh.index == -1) vertices[vh2.index].heh = heh1;

    return FaceHandle{static_cast<int64_t>(faces.size() - 1)};
}

FaceHandle HalfEdgeTable::addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2, VertexHandle vh3) {
    // Спрощена реалізація через розбиття квадрата на два трикутники
    FaceHandle f0 = addFace(vh0, vh1, vh2);
    FaceHandle f1 = addFace(vh0, vh2, vh3);
    return f0; // Повертаємо перший, або можна розширити для квадра
}
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); 
    }
};
void HalfEdgeTable::connectTwins() {
    std::unordered_map<std::pair<int64_t, int64_t>, HalfEdgeHandle, pair_hash> edgeMap;

    for (HalfEdgeHandle heh{0}; heh.index < static_cast<int64_t>(halfEdges.size()); ++heh.index) {
        VertexHandle src = sourceVertex(heh);
        VertexHandle dst = destVertex(heh);
        edgeMap[{src.index, dst.index}] = heh;
    }

    for (HalfEdgeHandle heh{0}; heh.index < static_cast<int64_t>(halfEdges.size()); ++heh.index) {
        if (halfEdges[heh.index].twin.index == -1) {
            VertexHandle src = destVertex(heh);
            VertexHandle dst = sourceVertex(heh);
            auto it = edgeMap.find({src.index, dst.index});
            if (it != edgeMap.end()) {
                halfEdges[heh.index].twin = it->second;
                halfEdges[it->second.index].twin = heh;
            }
        }
    }
}

void HalfEdgeTable::deleteFace(FaceHandle fh) {
    faces[fh.index].isDeleted = !faces[fh.index].isDeleted;
}

// --- Accessor methods ---

HalfEdgeHandle HalfEdgeTable::prev(HalfEdgeHandle heh) const {
    return deref(heh).prev;
}

HalfEdgeHandle HalfEdgeTable::next(HalfEdgeHandle heh) const {
    return deref(heh).next;
}

HalfEdgeHandle HalfEdgeTable::twin(HalfEdgeHandle heh) const {
    return deref(heh).twin;
}

VertexHandle HalfEdgeTable::destVertex(HalfEdgeHandle heh) const {
    return deref(heh).dst;
}

VertexHandle HalfEdgeTable::sourceVertex(HalfEdgeHandle heh) const {
    return destVertex(twin(heh));
}

HalfEdge& HalfEdgeTable::deref(HalfEdgeHandle heh) {
    return halfEdges[heh.index];
}

const HalfEdge& HalfEdgeTable::deref(HalfEdgeHandle heh) const {
    return halfEdges[heh.index];
}

HalfEdgeHandle HalfEdgeTable::handle(const HalfEdge& he) const {
    return HalfEdgeHandle{static_cast<int64_t>(&he - halfEdges.data())};
}

Vertex& HalfEdgeTable::deref(VertexHandle vh) {
    return vertices[vh.index];
}

const Vertex& HalfEdgeTable::deref(VertexHandle vh) const {
    return vertices[vh.index];
}

VertexHandle HalfEdgeTable::handle(const Vertex& v) const {
    return VertexHandle{static_cast<int64_t>(&v - vertices.data())};
}

Face& HalfEdgeTable::deref(FaceHandle fh) {
    return faces[fh.index];
}

const Face& HalfEdgeTable::deref(FaceHandle fh) const {
    return faces[fh.index];
}

FaceHandle HalfEdgeTable::handle(const Face& f) const {
    return FaceHandle{static_cast<int64_t>(&f - faces.data())};
}

// --- Position operations ---

const glm::vec3& HalfEdgeTable::getPoint(VertexHandle vh) const {
    return points[vh.index];
}

void HalfEdgeTable::setPoint(VertexHandle vh, glm::vec3 pos) {
    points[vh.index] = pos;
}

const glm::vec3& HalfEdgeTable::getStartPoint(HalfEdgeHandle heh) const {
    return getPoint(sourceVertex(heh));
}

void HalfEdgeTable::setStartPoint(HalfEdgeHandle heh, glm::vec3 pos) {
    setPoint(sourceVertex(heh), pos);
}

const glm::vec3& HalfEdgeTable::getEndPoint(HalfEdgeHandle heh) const {
    return getPoint(destVertex(heh));
}

void HalfEdgeTable::setEndPoint(HalfEdgeHandle heh, glm::vec3 pos) {
    setPoint(destVertex(heh), pos);
}

const std::vector<Vertex>& HalfEdgeTable::getVertices() const {
    return vertices;
}

const std::vector<Face>& HalfEdgeTable::getFaces() const {
    return faces;
}
