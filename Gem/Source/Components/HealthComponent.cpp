
#include <Source/Components/HealthComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Component/Entity.h>

namespace xXGameProjectNameXx
{
    void HealthComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<HealthComponent, HealthComponentBase>()
                ->Version(1)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<HealthComponent>("HealthComponent", "Provides the concept of health to its entity.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ;
            }
        }

        HealthComponentBase::Reflect(context);
    }

    void HealthComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        HealthComponentBase::GetRequiredServices(required);
    }

    void HealthComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        HealthComponentBase::GetDependentServices(dependent);
    }

    void HealthComponent::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }

    void HealthComponent::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }
} // namespace xXGameProjectNameXx


namespace xXGameProjectNameXx
{
    HealthComponentController::HealthComponentController(HealthComponent& parent)
        : HealthComponentControllerBase(parent)
    {
    }

    void HealthComponentController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        HealthRequestBus::Handler::BusConnect(GetEntityId());
    }

    void HealthComponentController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        HealthRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    float HealthComponentController::GetHealthValue() const
    {
        return GetHealth();
    }

#if AZ_TRAIT_SERVER
    void HealthComponentController::SetHealthValue(float newValue)
    {
        SetHealth(newValue);
    }
#endif
} // namespace xXGameProjectNameXx
