
#pragma once

#include <Source/AutoGen/HealthComponent.AutoComponent.h>
#include <Multiplayer/MultiplayerTypes.h>
#include <xXGameProjectNameXx/HealthInterface.h>

namespace AZ
{
    class Transform;
}

namespace xXGameProjectNameXx
{
    //! @class HealthComponent
    //! @brief
    class HealthComponent
        : public HealthComponentBase
    {
    public:

        AZ_MULTIPLAYER_COMPONENT(HealthComponent, s_healthComponentConcreteUuid, HealthComponentBase);

        //! Component descriptor interface.
        //! @{
        static void Reflect(AZ::ReflectContext* context);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        //! @}

        //! HealthComponentBase public overrides.
        //! @{
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        //! @}
    };
} // namespace xXGameProjectNameXx

namespace xXGameProjectNameXx
{
    //! @class HealthComponentController
    class HealthComponentController
        : public HealthComponentControllerBase
        , protected HealthRequestBus::Handler
    {
    public:

        explicit HealthComponentController(HealthComponent& parent);

        //! HealthComponentControllerBase public overrides.
        //! @{
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        //! @}

    protected:

        //! HealthRequestBus::Handler overrides.
        //! @{
        float GetHealthValue() const override;
#if AZ_TRAIT_SERVER
        void SetHealthValue(float newValue) override;
#endif
        //! @}

        AZ_INLINE ComponentType& GetOwnerCasted()
        {
            return static_cast<ComponentType&>(GetOwner());
        }
    };
} // namespace xXGameProjectNameXx
