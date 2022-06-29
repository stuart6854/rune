#pragma once

#include "rune/defines.hpp"
#include "rune/utility/guid.hpp"

#include <string>

namespace Rune
{
    enum class AssetType : u8
    {
        eNone = 0,
        eMesh,
        eTexture,
        eMaterial,
        eCount
    };

    auto assetTypeFromFileExt(const std::string& ext) -> AssetType;

    class Asset
    {
    public:
        virtual ~Asset() = default;

        auto getGuid() const -> const Guid&;
        auto getType() const -> AssetType;

    protected:
        Guid m_guid;
        AssetType m_type = AssetType::eNone;
    };
}
