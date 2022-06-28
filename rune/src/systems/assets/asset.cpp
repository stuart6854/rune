#include "rune/systems/assets/asset.hpp"

namespace Rune
{
    auto assetTypeFromFileExt(const std::string& ext) -> AssetType
    {
        if (ext == ".obj")
            return AssetType::eMesh;
        if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
            return AssetType::eTexture;

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
