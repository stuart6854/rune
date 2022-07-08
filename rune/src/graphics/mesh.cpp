#include "rune/graphics/mesh.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Mesh::~Mesh()
    {
        auto* renderer = GraphicsSystem::getInstance().getRenderer();
        renderer->destroyMesh(m_id);
    }

    auto Mesh::getIndices() const -> const std::vector<u16>&
    {
        return m_indices;
    }

    auto Mesh::getVertices() const -> const std::vector<Vertex>&
    {
        return m_vertices;
    }

    auto Mesh::getTopology() const -> MeshTopology
    {
        return m_topology;
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
        auto* renderer = GraphicsSystem::getInstance().getRenderer();

        if (m_id == 0)
            m_id = renderer->createMesh(m_vertices, m_indices, m_topology);
        else
        {
            renderer->updateMeshVertices(m_id, m_vertices);
            renderer->updateMeshIndices(m_id, m_indices);
        }
    }

    auto Mesh::getId() const -> u32
    {
        return m_id;
    }
}
