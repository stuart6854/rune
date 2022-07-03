#pragma once

#include "asset.hpp"
#include "rune/macros.hpp"

#include <string>
#include <vector>

namespace Rune
{
    class AssetFactory
    {
    public:
        virtual ~AssetFactory() = default;

        virtual auto createFromFile(const std::string& filename) -> Owned<Asset> = 0;
    };

    class TextureFactory : public AssetFactory
    {
    public:
        auto createFromFile(const std::string& filename) -> Owned<Asset> override;
    };

    class MeshFactory : public AssetFactory
    {
    public:
        auto createFromFile(const std::string& filename) -> Owned<Asset> override;

    private:
    };

    class ShaderFactory : public AssetFactory
    {
    public:
        auto createFromFile(const std::string& filename) -> Owned<Asset> override;

    private:
        static auto readShaderSource(const std::string& filename) -> std::vector<u8>;
    };

}
