
#pragma once

#include <xXGameProjectNameXx/xXGameProjectNameXxTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Component/EntityId.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>

namespace xXGameProjectNameXx
{
    class GameEntityRequests
    {
    public:

        AZ_RTTI(GameEntityRequests, GetTypeId<GameEntityRequests>());

        virtual ~GameEntityRequests() = default;

        //! @brief Gets the current game entity's entity id.
        virtual AZ::EntityId GetGameEntityId() const = 0;

        //! @brief Gets the current game entity's network entity handle.
        virtual Multiplayer::ConstNetworkEntityHandle GetGameEntityNetworkHandle() const = 0;
    };

    class GameEntityBusTraits
        : public AZ::EBusTraits
    {
    public:

        //! AZ::EBusTraits overrides.
        //! @{
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //! @}
    };

    using GameEntityRequestBus = AZ::EBus<GameEntityRequests, GameEntityBusTraits>;
    using GameEntityInterface = AZ::Interface<GameEntityRequests>;

} // namespace xXGameProjectNameXx
