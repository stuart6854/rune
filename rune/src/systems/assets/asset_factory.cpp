#include "rune/systems/assets/asset_factory.hpp"

#include "rune/macros.hpp"
#include "rune/systems/graphics/texture.hpp"
#include "rune/systems/graphics/mesh.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "assimp/scene.h"

#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Rune
{
    auto formatFromChannels(const i32 c) -> TextureFormat
    {
        if (c == 1)
            return TextureFormat::eR;
        if (c == 3)
            return TextureFormat::eRGB;
        if (c == 4)
            return TextureFormat::eRGBA;

        return TextureFormat::eUnknown;
    }

    auto TextureFactory::createFromFile(const std::string& filename) -> Owned<Asset>
    {
        // Create texture
        auto texture = CreateOwned<Texture>();

        // Enable flipping texture on load
        stbi_set_flip_vertically_on_load(true);

        // Load texture file
        i32 w, h, c;
        auto* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
        if (data == nullptr)
        {
            CORE_LOG_ERROR("Failed to load texture file: {}", filename);
            return nullptr;
        }

        // Get texture format from # of channels
        TextureFormat format = formatFromChannels(c);
        if (format == TextureFormat::eUnknown)
        {
            CORE_LOG_ERROR("Trying to load unsupported texture format! channels={}", c);
            stbi_image_free(data);
            return nullptr;
        }

        // Init texture with loaded data
        texture->init(w, h, format, data);

        // Free texture data
        stbi_image_free(data);

        return std::move(texture);
    }

    auto MeshFactory::createFromFile(const std::string& filename) -> Owned<Asset>
    {
        constexpr auto importFlags =
            aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs | aiProcess_PreTransformVertices | aiProcess_OptimizeMeshes;

        // Create mesh
        auto newMesh = CreateOwned<Mesh>();

        // Import mesh file
        Assimp::Importer importer;
        const auto* scene = importer.ReadFile(filename.c_str(), importFlags);
        if (scene == nullptr)
        {
            CORE_LOG_ERROR("Failed to load mesh file: {}\n{}", filename, importer.GetErrorString());
            return nullptr;
        }

        // Check if scene has meshes
        if (!scene->HasMeshes())
        {
            CORE_LOG_ERROR("Mesh file does not contain any meshes: {}", filename);
            return nullptr;
        }

        std::vector<Vertex> vertices;
        size vertexCount = 0;
        std::vector<u16> indices;
        size indexCount = 0;

        for (size meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const auto* submesh = scene->mMeshes[meshIndex];

            // Vertices
            vertices.resize(vertexCount + submesh->mNumVertices);
            for (size vertIndex = 0; vertIndex < submesh->mNumVertices; ++vertIndex)
            {
                if (vertexCount > u16_max)
                {
                    CORE_LOG_ERROR("Only 16bit indices are supported! So cannot have more than {} vertices!", u16_max);
                    break;
                }

                auto& newVertex = vertices[vertexCount];

                const auto& vert = submesh->mVertices[vertIndex];
                newVertex.pos = { vert.x, vert.y, vert.z };

                const auto& uv = submesh->mTextureCoords[0][vertIndex];
                newVertex.uv = { uv.x, uv.y };

                const auto& norm = submesh->mNormals[vertIndex];
                newVertex.norm = { norm.x, norm.y, norm.z };

                ++vertexCount;
            }

            // Set Submesh
            Mesh::Submesh newSubmesh{};
            newSubmesh.firstIndex = indexCount;
            newSubmesh.indexCount = submesh->mNumFaces * 3;
            newMesh->setSubmesh(meshIndex, newSubmesh);

            // Triangles
            indices.resize(indexCount + submesh->mNumFaces * 3);
            for (size faceIndex = 0; faceIndex < submesh->mNumFaces; faceIndex++)
            {
                const auto& face = submesh->mFaces[faceIndex];

                RUNE_ENG_ASSERT(face.mNumIndices == 3, "Faces should consist of 3 vertices!");

                indices[indexCount] = face.mIndices[0];
                indices[indexCount + 1] = face.mIndices[1];
                indices[indexCount + 2] = face.mIndices[2];

                indexCount += 3;
            }
        }

        CORE_LOG_TRACE("Mesh loaded {}", filename);
        CORE_LOG_TRACE("  submeshes  =  {}", scene->mNumMeshes);
        CORE_LOG_TRACE("   vertices  =  {}", vertexCount);
        CORE_LOG_TRACE("    indices  =  {}", indexCount);

        // Init mesh with loaded data
        newMesh->setVertices(vertices);
        newMesh->setIndices(indices, MeshTopology::eTriangles);
        newMesh->apply();

        return std::move(newMesh);
    }
}
