
#pragma once

#include <xXGameProjectNameXx/xXGameProjectNameXxTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace xXGameProjectNameXx
{
    class GameEntityRequests
    {
    public:

        AZ_RTTI(GameEntityRequests, GetTypeId<GameEntityRequests>());

        virtual ~GameEntityRequests() = default;
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
