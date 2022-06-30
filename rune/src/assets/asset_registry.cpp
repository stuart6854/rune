#include "rune/assets/asset_registry.hpp"

#include <filesystem>

namespace Rune
{
    auto AssetRegistry::getInstance() -> AssetRegistry&
    {
        static AssetRegistry assetRegistry;
        return assetRegistry;
    }

    auto AssetRegistry::add(const std::string& filename) -> AssetHandle
    {
        std::filesystem::path filePath = filename;

        Guid newGuid;
        m_assetGuidMap[filename] = newGuid;
        auto& metadata = m_assetMetadata[newGuid];
        metadata.guid = newGuid;
        metadata.sourceFile = filename;
        metadata.type = assetTypeFromFileExt(filePath.extension().string());

        return newGuid;
    }

    auto AssetRegistry::add(const std::string& name, const Owned<Asset>& asset) -> AssetHandle
    {
        auto& guid = asset->getGuid();

        // Map asset name to its Guid
        m_assetGuidMap[name] = guid;

        // Setup metadata
        auto& metadata = m_assetMetadata[guid];
        metadata.guid = guid;
        metadata.type = asset->getType();

        return guid;
    }

    auto AssetRegistry::getHandle(const std::string& name) -> AssetHandle
    {
        const auto it = m_assetGuidMap.find(name);
        if (it != m_assetGuidMap.end())
            return it->second;

        CORE_LOG_ERROR("No asset with name: {}", name);
        return 0;
    }

    void AssetRegistry::load(AssetHandle handle)
    {
        // Check if an asset exists for this handle
        auto it = m_assetMetadata.find(handle);
        if (it == m_assetMetadata.end())
        {
            CORE_LOG_ERROR("No asset to load for handle {{}}", handle);
            return;
        }

        auto& metadata = it->second;

        // Get the factory for the assets type
        const auto& factory = m_assetFactories[static_cast<i8>(metadata.type)];
        if (factory == nullptr)
        {
            CORE_LOG_ERROR("No asset factory registered for asset type!");
            return;
        }

        // This asset source is not loaded from a file
        if (metadata.sourceFile.empty())
            return;

        // Create the asset from file using the factory
        auto asset = factory->createFromFile(metadata.sourceFile);
        // Check that the asset loaded correctly
        if (asset == nullptr)
            return;

        // Store the loaded asset
        m_loadedAssets[handle] = std::move(asset);

        // Mark the asset as loaded in its metadata
        metadata.isLoaded = true;
    }

    void AssetRegistry::unload(AssetHandle handle)
    {
        // Check if an asset exists for this handle
        auto metadataIt = m_assetMetadata.find(handle);
        if (metadataIt == m_assetMetadata.end())
        {
            CORE_LOG_ERROR("No asset to unload for handle {{}}", handle);
            return;
        }

        auto& metadata = metadataIt->second;

        // This asset source is not loaded from a file
        if (metadata.sourceFile.empty())
            return;

        auto assetIt = m_loadedAssets.find(handle);
        if (assetIt != m_loadedAssets.end())
        {
            // Destroy the asset
            auto& asset = assetIt->second;
            asset.reset();
            // Remove it from list of loaded assets
            m_loadedAssets.erase(assetIt);
        }

        // Make sure the asset it NOT marked as loaded
        metadata.isLoaded = false;
    }
}
