#include "rune/graphics/mesh.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Mesh::~Mesh()
    {
        onDestroying();
    }

    void Mesh::attachObserver(Observer* observer)
    {
        m_observers.push_back(observer);
    }

    void Mesh::detachObserver(Observer* observer)
    {
        const auto& it = std::ranges::find(m_observers, observer);
        if (it != m_observers.end())
            m_observers.erase(it);
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

    void Mesh::apply() const
    {
        onChanged();
    }

    void Mesh::onDestroying() const
    {
        for (const auto& observer : m_observers)
        {
            observer->destroying(this);
        }
    }

    void Mesh::onChanged() const
    {
        for (const auto& observer : m_observers)
        {
            observer->changed(this);
        }
    }
}
