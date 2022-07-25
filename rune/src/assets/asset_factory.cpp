// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/assets/asset_factory.hpp"

#include "rune/macros.hpp"
#include "rune/graphics/texture.hpp"
#include "rune/graphics/mesh.hpp"
#include "rune/graphics/shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

#include <filesystem>

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

        std::vector<u8> pixels;
        pixels.assign(data, data + w * h * c);

        // Init texture with loaded data
        texture->init(w, h, format, pixels);

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

    auto ShaderFactory::createFromFile(const std::string& filename) -> Owned<Asset>
    {
        auto shader = CreateOwned<Shader>();

        auto result = toml::parse_file(filename);
        if (!result)
        {
            auto err = result.error().description();
            CORE_LOG_ERROR("Failed to parse config file: {}\n{}", filename, err);
            return nullptr;
        }

        auto& shaderDef = result.table();
        auto& isCompiled = shaderDef["is_compiled"].as_boolean()->get();
        auto& vertexFile = shaderDef["vertex"].as_string()->get();
        auto& fragmentFile = shaderDef["fragment"].as_string()->get();

        std::filesystem::path assetDirPath = filename;
        assetDirPath = assetDirPath.parent_path();

        shader->setIsCompiled(isCompiled);
        shader->setVertexCode(readShaderSource(assetDirPath.string() + "/" + vertexFile));
        shader->setFragmentCode(readShaderSource(assetDirPath.string() + "/" + fragmentFile));
        shader->reflect();

        return std::move(shader);
    }

    auto ShaderFactory::readShaderSource(const std::string& filename) -> std::vector<u8>
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            CORE_LOG_ERROR("Failed to open shader file: {}", filename);
            return {};
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<u8> buffer(fileSize);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

        file.close();

        return buffer;
    }
}
