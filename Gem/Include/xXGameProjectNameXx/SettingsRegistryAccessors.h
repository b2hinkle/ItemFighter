
#pragma once

#include <xXGameProjectNameXx/xXGameProjectNameXxConfiguration.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/std/optional.h>

namespace AZ
{
    namespace Data
    {
        template <class T>
        class Asset;
    }
}

namespace AzFramework
{
    class Spawnable;
}

namespace xXGameProjectNameXx::SettingsRegistryAccessors
{
#if AZ_TRAIT_CLIENT
    namespace Canvases
    {
        constexpr AZStd::string_view StageSelectCanvasPathnameRegistryPath = "/xXGameProjectNameXx/Canvases/StageSelectPathname";

        XXGAMEPROJECTNAMEXX_API AZ::SettingsRegistryInterface::FixedValueString GetStageSelectCanvasPathname();
    }
#endif // #if AZ_TRAIT_CLIENT

#if AZ_TRAIT_CLIENT
    namespace UI
    {
        constexpr AZStd::string_view CursorTexturePathnameRegistryPath = "/xXGameProjectNameXx/UI/CursorTexturePathname";

        XXGAMEPROJECTNAMEXX_API AZ::SettingsRegistryInterface::FixedValueString GetCursorTexturePathname();
    }
#endif // #if AZ_TRAIT_CLIENT

    constexpr AZStd::string_view LevelGameEntityPrefabPathname = "/xXGameProjectNameXx/LevelGameEntityNetworkSpawnablePathname";

    XXGAMEPROJECTNAMEXX_API AZStd::optional<AZ::SettingsRegistryInterface::FixedValueString> TryGetLevelGameEntityPrefabPathname();
    XXGAMEPROJECTNAMEXX_API AZStd::optional<AZ::Data::Asset<AzFramework::Spawnable>> TryGetLevelGameEntityPrefabAsset();
} // namespace xXGameProjectNameXx
