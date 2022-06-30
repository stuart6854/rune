#include "rune/graphics/mesh.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Mesh::~Mesh()
    {
        auto& graphicsInst = GraphicsSystem::getInstance();
        graphicsInst.destroyMesh(m_internalMesh);
    }

    void Mesh::setVertices(const std::vector<Vertex>& vertices)
    {
        m_vertices = vertices;
    }

    void Mesh::setIndices(const std::vector<u16>& indices, const MeshTopology topology)
    {
        m_topology = topology;
        m_indices = indices;
    }

    void Mesh::setSubmesh(const size index, const Submesh& submesh)
    {
        // Resize array
        if (m_submeshes.size() <= index)
            m_submeshes.resize(index + 1);

        // Assign submesh
        m_submeshes[index] = submesh;
    }

    void Mesh::apply()
    {
        auto& graphicsInst = GraphicsSystem::getInstance();
        m_internalMesh = graphicsInst.createMesh(MeshTopology::eTriangles, m_vertices, m_indices);
    }
}
