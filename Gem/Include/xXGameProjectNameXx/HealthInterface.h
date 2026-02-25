
#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Multiplayer
{
    struct NetworkSpawnable;
}

namespace xXGameProjectNameXx
{
    namespace HealthRequestFunctions
    {
        float GetHealthValue(const AZ::EntityId& entityId);
#if AZ_TRAIT_SERVER
        void SetHealthValue(const AZ::EntityId& entityId, float newValue);
#endif
    }

    class HealthRequests
        : public AZ::ComponentBus
    {
    public:

        AZ_RTTI(xXGameProjectNameXx::HealthRequests, "{9FCE2ACA-6FEC-4555-8546-D693C6FC7366}");

        virtual float GetHealthValue() const = 0;
#if AZ_TRAIT_SERVER
        virtual void SetHealthValue(float newValue) = 0;
#endif
    };

    using HealthRequestBus = AZ::EBus<HealthRequests>;

    namespace HealthRequestFunctions
    {
        AZ_FORCE_INLINE float GetHealthValue(const AZ::EntityId& entityId)
        {
            float result{};
            HealthRequestBus::EventResult(result, entityId, &HealthRequestBus::Events::GetHealthValue);
            return result;
        }

#if AZ_TRAIT_SERVER
        AZ_FORCE_INLINE void SetHealthValue(const AZ::EntityId& entityId, float newValue)
        {
            HealthRequestBus::Event(entityId, &HealthRequestBus::Events::SetHealthValue, newValue);
        }
#endif
    }
} // namespace xXGameProjectNameXx
