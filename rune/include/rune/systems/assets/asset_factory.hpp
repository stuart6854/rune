#pragma once

#include "asset.hpp"

#include <string>

namespace Rune
{
    class AssetFactory
    {
    public:
        virtual ~AssetFactory() = default;

        virtual auto createFromFile(const std::string& filename) -> Owned<Asset> = 0;
    };

    class MeshFactory : public AssetFactory
    {
    public:
        auto createFromFile(const std::string& filename) -> Owned<Asset> override
        {
            return nullptr;
        }
    };
}
