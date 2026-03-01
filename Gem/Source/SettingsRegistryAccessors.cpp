
#include <xXGameProjectNameXx/SettingsRegistryAccessors.h>

#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/Console/ILogger.h>
#include <AzCore/Debug/Trace.h>
#include <AzCore/Asset/AssetManagerBus.h>
#include <Multiplayer/MultiplayerTypes.h>

namespace xXGameProjectNameXx::SettingsRegistryAccessors
{
#if AZ_TRAIT_CLIENT
    namespace Canvases
    {
        AZ::SettingsRegistryInterface::FixedValueString GetStageSelectCanvasPathname()
        {
            const AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
            AZ_Assert(settingsRegistry, "Should be valid.");

            AZ::SettingsRegistryInterface::FixedValueString result;
            const bool hasRetrievedValue = settingsRegistry->Get(result, StageSelectCanvasPathnameRegistryPath);

            if (!hasRetrievedValue)
            {
                AZStd::fixed_string<256> message;
                message += "No value specified for settings registry path '";
                message += StageSelectCanvasPathnameRegistryPath;
                message += "'.";

                AZLOG_ERROR(message.data());
            }

            return result;
        }
    }
#endif // #if AZ_TRAIT_CLIENT

#if AZ_TRAIT_CLIENT
    namespace UI
    {
        AZ::SettingsRegistryInterface::FixedValueString GetCursorTexturePathname()
        {
            const AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
            AZ_Assert(settingsRegistry, "Should be valid.");

            AZ::SettingsRegistryInterface::FixedValueString result;
            const bool hasRetrievedValue = settingsRegistry->Get(result, CursorTexturePathnameRegistryPath);

            if (!hasRetrievedValue)
            {
                AZStd::fixed_string<256> message;
                message += "No value specified for settings registry path '";
                message += CursorTexturePathnameRegistryPath;
                message += "'.";

                AZLOG_ERROR(message.data());
            }

            return result;
        }
    }
#endif // #if AZ_TRAIT_CLIENT

    AZStd::optional<AZ::SettingsRegistryInterface::FixedValueString> TryGetLevelGameEntityPrefabPathname()
    {
        const AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
        AZ_Assert(settingsRegistry, "Should be valid.");

        AZ::SettingsRegistryInterface::FixedValueString result;
        const bool hasRetrievedValue = settingsRegistry->Get(result, LevelGameEntityPrefabPathname);

        if (!hasRetrievedValue)
        {
            AZStd::fixed_string<256> message;
            message += "No value specified for settings registry path '";
            message += LevelGameEntityPrefabPathname;
            message += "'. Returning nullopt.";

            AZLOG_INFO(message.data());
            return AZStd::nullopt;
        }

        return result;
    }

    AZStd::optional<AZ::Data::Asset<AzFramework::Spawnable>> TryGetLevelGameEntityPrefabAsset()
    {
        AZStd::optional path = SettingsRegistryAccessors::TryGetLevelGameEntityPrefabPathname();
        if (!path.has_value())
        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "No settings registry value specified. Returning nullopt.";

            AZLOG_INFO(logString.data());
            return AZStd::nullopt;
        }

        AZ::Data::AssetType typeToRegister = azrtti_typeid<AzFramework::Spawnable>();
        constexpr bool autoRegisterIfNotFound = false;

        AZ::Data::AssetId result{};
        AZ::Data::AssetCatalogRequestBus::BroadcastResult(
            result,
            &AZ::Data::AssetCatalogRequestBus::Events::GetAssetIdByPath,
            path->c_str(),
            typeToRegister,
            autoRegisterIfNotFound);

        if (!result.IsValid())
        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "Settings registry's level game entity prefab pathname gave us an invalid asset id. Returning nullopt.";

            AZLOG_ERROR(logString.data());
            return AZStd::nullopt;
        }

        return AZ::Data::Asset<AzFramework::Spawnable>{
            result,
            azrtti_typeid<AzFramework::Spawnable>(),
            AZStd::string{*path}
        };
    }
} // namespace xXGameProjectNameXx::Canvases
