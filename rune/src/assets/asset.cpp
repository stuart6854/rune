#include "pch.hpp"
#include "rune/assets/asset.hpp"

namespace Rune
{
    auto assetTypeFromFileExt(const std::string& ext) -> AssetType
    {
        if (ext == ".obj" || ext == ".fbx" || ext == ".gltf")
            return AssetType::eMesh;
        if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
            return AssetType::eTexture;
        if (ext == ".shader")
            return AssetType::eShader;
        if (ext == ".mat")
            return AssetType::eMaterial;

        return AssetType::eNone;
    }

    auto Asset::getGuid() const -> const Guid&
    {
        return m_guid;
    }

    auto Asset::getType() const -> AssetType
    {
        return m_type;
    }
}
