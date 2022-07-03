#pragma once

#include "rune/defines.hpp"
#include "rune/macros.hpp"
#include "asset.hpp"
#include "asset_factory.hpp"

#include <array>
#include <map>
#include <unordered_map>

namespace Rune
{
    using AssetHandle = Guid;
    constexpr u64 NULL_ASSET = 0;

    struct AssetMetadata
    {
        Guid guid = NULL_ASSET;
        AssetType type = AssetType::eNone;
        std::string sourceFile;

        bool isLoaded = false;
    };

    class AssetRegistry
    {
    public:
        static auto getInstance() -> AssetRegistry&;

        template <typename FactoryType>
        void registerFactory(AssetType assetType);

        // TODO: init/cleanup

        auto add(const std::string& filename) -> AssetHandle;
        auto add(const std::string& name, Owned<Asset> asset) -> AssetHandle;

        auto getHandle(const std::string& name) -> AssetHandle;

        void load(AssetHandle handle);
        void unload(AssetHandle handle);

        template <typename T>
        auto get(AssetHandle handle) -> T*;

    private:
        std::array<Owned<AssetFactory>, static_cast<i8>(AssetType::eCount)> m_assetFactories;

        std::unordered_map<std::string, Guid> m_assetGuidMap;
        std::map<Guid, AssetMetadata> m_assetMetadata;
        std::map<Guid, Owned<Asset>> m_loadedAssets;
    };

    template <typename FactoryType>
    void AssetRegistry::registerFactory(AssetType assetType)
    {
        static_assert(std::is_base_of_v<AssetFactory, FactoryType>);

        auto index = static_cast<i8>(assetType);
        m_assetFactories[index] = CreateOwned<FactoryType>();
    }

    template <typename T>
    auto AssetRegistry::get(const AssetHandle handle) -> T*
    {
        auto metadataIt = m_assetMetadata.find(handle);
        if (metadataIt == m_assetMetadata.end())
        {
            CORE_LOG_WARN("No asset for handle {{}}", handle);
            return nullptr;
        }

        const auto& metadata = metadataIt->second;
        if (!metadata.isLoaded)
        {
            CORE_LOG_WARN("Asset not loaded for handle {{}}", handle);
            return nullptr;
        }

        auto assetIt = m_loadedAssets.find(handle);
        if (assetIt == m_loadedAssets.end())
        {
            return nullptr;
        }

        auto* assetPtr = assetIt->second.get();
        return static_cast<T*>(assetPtr);
    }
}
