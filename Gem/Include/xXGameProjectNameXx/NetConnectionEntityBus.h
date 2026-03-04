
#pragma once

#include <xXGameProjectNameXx/xXGameProjectNameXxTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Component/EntityId.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>
#include <xXGameProjectNameXx/xXGameProjectNameXxConfiguration.h>
#include <cstdint>
#include <AzNetworking/ConnectionLayer/IConnection.h>

namespace xXGameProjectNameXx
{
    XXGAMEPROJECTNAMEXX_API AZ::EntityId GetLocalNetConnectionEntityId();
    XXGAMEPROJECTNAMEXX_API Multiplayer::ConstNetworkEntityHandle GetLocalNetConnectionEntityNetworkHandle();

#if AZ_TRAIT_SERVER
    XXGAMEPROJECTNAMEXX_API AZ::EntityId GetNetConnectionEntityIdByMultiplayerUserId(std::uint64_t temporaryUserId);
    XXGAMEPROJECTNAMEXX_API Multiplayer::ConstNetworkEntityHandle GetNetConnectionEntityNetworkHandleByMultiplayerUserId(std::uint64_t temporaryUserId);

    XXGAMEPROJECTNAMEXX_API AZ::EntityId GetNetConnectionEntityIdByConnectionId(AzNetworking::ConnectionId connectionId);
    XXGAMEPROJECTNAMEXX_API Multiplayer::ConstNetworkEntityHandle GetNetConnectionEntityNetworkHandleByConnectionId(AzNetworking::ConnectionId connectionId);
#endif // #if AZ_TRAIT_SERVER

    class NetConnectionEntityRequests
    {
    public:

        AZ_RTTI(NetConnectionEntityRequests, GetTypeId<NetConnectionEntityRequests>());

        virtual ~NetConnectionEntityRequests() = default;

        virtual AZ::EntityId GetLocalNetConnectionEntityId() const = 0;
        virtual Multiplayer::ConstNetworkEntityHandle GetLocalNetConnectionEntityNetworkHandle() const = 0;

#if AZ_TRAIT_SERVER
        virtual AZ::EntityId GetNetConnectionEntityIdByMultiplayerUserId(std::uint64_t temporaryUserId) const = 0;
        virtual Multiplayer::ConstNetworkEntityHandle GetNetConnectionEntityNetworkHandleByMultiplayerUserId(std::uint64_t temporaryUserId) const = 0;

        virtual AZ::EntityId GetNetConnectionEntityIdByConnectionId(AzNetworking::ConnectionId connectionId) const = 0;
        virtual Multiplayer::ConstNetworkEntityHandle GetNetConnectionEntityNetworkHandleByConnectionId(AzNetworking::ConnectionId connectionId) const = 0;
#endif // #if AZ_TRAIT_SERVER
    };

    class NetConnectionEntityBusTraits
        : public AZ::EBusTraits
    {
    public:

        //! AZ::EBusTraits overrides.
        //! @{
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //! @}
    };

    using NetConnectionEntityRequestBus = AZ::EBus<NetConnectionEntityRequests, NetConnectionEntityBusTraits>;
    using NetConnectionEntityInterface = AZ::Interface<NetConnectionEntityRequests>;

} // namespace xXGameProjectNameXx
