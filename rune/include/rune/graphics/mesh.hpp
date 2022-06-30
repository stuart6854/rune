#pragma once

#include "rune/assets/asset.hpp"
#include "vertex.hpp"

#include <vector>

namespace Rune
{
    enum class MeshTopology : i8
    {
        eNone,
        eLines,
        eTriangles
    };

    class Mesh : public Asset
    {
    public:
        struct Submesh
        {
            i32 firstIndex;
            i32 indexCount;
        };

    public:
        ~Mesh() override;

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<u16>& indices, MeshTopology topology);
        void setSubmesh(size index, const Submesh& submesh);

        void apply();

    private:
        // std::vector<Material> m_materials;

        std::vector<u16> m_indices;
        std::vector<Vertex> m_vertices;
        std::vector<Submesh> m_submeshes;
        MeshTopology m_topology = MeshTopology::eNone;

        u32 m_internalMesh{};
    };
}
