#include "Mesh.h"

Mesh::Mesh(const HalfEdgeTable& halfEdgeTable)
    : m_halfEdgeTable(halfEdgeTable)
{
    // Обчислюємо bounding box
    const auto& vertices = m_halfEdgeTable.getVertices();

    if (vertices.empty())
    {
        m_bbox.min = glm::vec3(0.0f);
        m_bbox.max = glm::vec3(0.0f);
        return;
    }

    glm::vec3 minPt = m_halfEdgeTable.getPoint(VertexHandle{0});
    glm::vec3 maxPt = m_halfEdgeTable.getPoint(VertexHandle{0});

    for (size_t i = 1; i < vertices.size(); ++i)
    {
        if (vertices[i].isDeleted) continue;
        glm::vec3 pos = m_halfEdgeTable.getPoint(VertexHandle{ static_cast<int64_t>(i) });

        minPt = glm::min(minPt, pos);
        maxPt = glm::max(maxPt, pos);
    }

    m_bbox.min = minPt;
    m_bbox.max = maxPt;
}

void Mesh::render(IRenderSystem& rs, glm::vec3& color)
{
    std::vector<VertexSimple> vertices;

    const auto& faces = m_halfEdgeTable.getFaces();

    for (const auto& face : faces)
    {
        if (face.isDeleted) continue;

        HalfEdgeHandle startHeh = face.heh;
        HalfEdgeHandle heh = startHeh;

        std::vector<glm::vec3> facePositions;

        do
        {
            VertexHandle vh = m_halfEdgeTable.destVertex(heh);
            facePositions.push_back(m_halfEdgeTable.getPoint(vh));
            heh = m_halfEdgeTable.next(heh);
        }
        while (heh.index != startHeh.index);

        if (facePositions.size() < 3)
            continue; // не можна побудувати трикутник

        // Нормаль обчислюємо один раз для всієї грані (Face)
        glm::vec3 normal(0.0f);
        {
            glm::vec3 edge1 = facePositions[1] - facePositions[0];
            glm::vec3 edge2 = facePositions[2] - facePositions[0];
            normal = glm::normalize(glm::cross(edge1, edge2));
        }

        // Якщо трикутник
        if (facePositions.size() == 3)
        {
            for (auto& pos : facePositions)
            {
                vertices.push_back({pos, normal});
            }
        }
        // Якщо чотирикутник, розбиваємо на два трикутники
        else if (facePositions.size() == 4)
        {
            glm::vec3 p0 = facePositions[0];
            glm::vec3 p1 = facePositions[1];
            glm::vec3 p2 = facePositions[2];
            glm::vec3 p3 = facePositions[3];

            // Перший трикутник
            vertices.push_back({p0, normal});
            vertices.push_back({p1, normal});
            vertices.push_back({p2, normal});

            // Другий трикутник
            vertices.push_back({p0, normal});
            vertices.push_back({p2, normal});
            vertices.push_back({p3, normal});
        }
    }

    rs.renderTriangleSoup(vertices, color);
}

const bbox& Mesh::getBoundingBox() const
{
    return m_bbox;
}

void Mesh::applyTransformation(FaceHandle fh, const glm::mat4& trf)
{
    if (fh.index < 0 || fh.index >= static_cast<int64_t>(m_halfEdgeTable.getFaces().size()))
        return;

    const Face& face = m_halfEdgeTable.deref(fh);
    if (face.isDeleted)
        return;

    HalfEdgeHandle startHeh = face.heh;
    HalfEdgeHandle heh = startHeh;

    do
    {
        VertexHandle vh = m_halfEdgeTable.destVertex(heh);
        glm::vec3 pos = m_halfEdgeTable.getPoint(vh);
        glm::vec4 transformedPos = trf * glm::vec4(pos, 1.0f);
        m_halfEdgeTable.setPoint(vh, glm::vec3(transformedPos));
        
        heh = m_halfEdgeTable.next(heh);
    } 
    while (heh.index != startHeh.index);

    // Переобчислюємо bounding box після трансформації
    const auto& vertices = m_halfEdgeTable.getVertices();

    glm::vec3 minPt(FLT_MAX);
    glm::vec3 maxPt(-FLT_MAX);

    for (const auto& vertex : vertices)
    {
        if (vertex.isDeleted) continue;
        glm::vec3 pos = m_halfEdgeTable.getPoint(VertexHandle{ &vertex - &vertices[0] });
        minPt = glm::min(minPt, pos);
        maxPt = glm::max(maxPt, pos);
    }

    m_bbox.min = minPt;
    m_bbox.max = maxPt;
}

void Mesh::deleteFace(FaceHandle fh)
{
    m_halfEdgeTable.deleteFace(fh);
}

const HalfEdgeTable& Mesh::getHalfEdgeTable() const
{
    return m_halfEdgeTable;
}

void Mesh::moveMesh(const glm::vec3& offset)
{
    for (auto& vertex : m_halfEdgeTable.getVertices())
    {
        if (!vertex.isDeleted)
        {
            glm::vec3 pos = m_halfEdgeTable.getPoint(m_halfEdgeTable.handle(vertex));
            m_halfEdgeTable.setPoint(m_halfEdgeTable.handle(vertex), pos + offset);
        }
    }

    m_bbox.min += offset;
    m_bbox.max += offset;
}
bool rayIntersectsTriangle(
    const glm::vec3& orig, const glm::vec3& dir,
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
    float& t
);
FaceHandle Mesh::intersectRay(const ray& r) const {
    FaceHandle closestFace = { -1 };
    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& face : m_halfEdgeTable.getFaces()) {
        if (face.isDeleted) continue;

        HalfEdgeHandle heh = face.heh;
        VertexHandle vh0 = m_halfEdgeTable.sourceVertex(heh);
        VertexHandle vh1 = m_halfEdgeTable.destVertex(heh);
        VertexHandle vh2 = m_halfEdgeTable.destVertex(m_halfEdgeTable.next(heh));

        glm::vec3 v0 = m_halfEdgeTable.getPoint(vh0);
        glm::vec3 v1 = m_halfEdgeTable.getPoint(vh1);
        glm::vec3 v2 = m_halfEdgeTable.getPoint(vh2);

        float t;
        if (rayIntersectsTriangle(r.orig, r.dir, v0, v1, v2, t)) {
            if (t < closestDistance) {
                closestDistance = t;
                closestFace = m_halfEdgeTable.handle(face);
            }
        }
    }

    return closestFace;
}

bool rayIntersectsTriangle(
    const glm::vec3& orig, const glm::vec3& dir,
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
    float& t
) {
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(dir, edge2);
    float a = glm::dot(edge1, h);
    if (fabs(a) < EPSILON)
        return false; // Промінь паралельний трикутнику

    float f = 1.0f / a;
    glm::vec3 s = orig - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(dir, q);
    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * glm::dot(edge2, q);
    return t > EPSILON; // Є перетин якщо t > 0
}