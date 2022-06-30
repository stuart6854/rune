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
        class Observer
        {
        public:
            virtual ~Observer() = default;

            virtual void destroying(const Mesh* mesh) = 0;
            virtual void changed(const Mesh* mesh) = 0;
        };

        struct Submesh
        {
            i32 firstIndex;
            i32 indexCount;
        };

    public:
        ~Mesh() override;

        void attachObserver(Observer* observer);
        void detachObserver(Observer* observer);

        auto getIndices() const -> const std::vector<u16>&;
        auto getVertices() const -> const std::vector<Vertex>&;
        auto getTopology() const -> MeshTopology;

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<u16>& indices, MeshTopology topology);
        void setSubmesh(size index, const Submesh& submesh);

        void apply() const;

    protected:
        void onDestroying() const;
        void onChanged() const;

    private:
        std::vector<Observer*> m_observers;

        // std::vector<Material> m_materials;

        std::vector<u16> m_indices;
        std::vector<Vertex> m_vertices;
        std::vector<Submesh> m_submeshes;
        MeshTopology m_topology = MeshTopology::eNone;

        u32 m_internalMesh{};
    };
}
