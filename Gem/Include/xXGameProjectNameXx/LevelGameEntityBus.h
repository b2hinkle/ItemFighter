
#pragma once

#include <xXGameProjectNameXx/xXGameProjectNameXxTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Component/EntityId.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>

namespace xXGameProjectNameXx
{
    class LevelGameEntityRequests
    {
    public:

        AZ_RTTI(LevelGameEntityRequests, GetTypeId<LevelGameEntityRequests>());

        virtual ~LevelGameEntityRequests() = default;

        //! @brief Gets the current level game entity's entity id.
        virtual AZ::EntityId GetLevelGameEntityId() const = 0;

        //! @brief Gets the current level game entity's network entity handle.
        virtual Multiplayer::ConstNetworkEntityHandle GetLevelGameEntityNetworkHandle() const = 0;
    };

    class LevelGameEntityBusTraits
        : public AZ::EBusTraits
    {
    public:

        //! AZ::EBusTraits overrides.
        //! @{
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //! @}
    };

    using LevelGameEntityRequestBus = AZ::EBus<LevelGameEntityRequests, LevelGameEntityBusTraits>;
    using LevelGameEntityInterface = AZ::Interface<LevelGameEntityRequests>;

} // namespace xXGameProjectNameXx
